#include "../inc/cameracalibrator.hpp"

using namespace std;
using namespace cv;
using namespace boost::filesystem;

vector<string> CameraCalibrator::getImagesFromDir(string filepath) {
	struct stat s;
	vector<string> images;
	if (stat(filepath.c_str(), &s) == 0) {
		if (S_ISDIR(s.st_mode)) {
			path p(filepath);
			if (exists(p)) {
				typedef vector<boost::filesystem::path> vec; // store paths
				vec v; // so we can sort them later
				copy(directory_iterator(p), directory_iterator(),
						back_inserter(v));
				sort(v.begin(), v.end());
				for (vec::const_iterator it(v.begin()), it_end(v.end()); it
						!= it_end; ++it) {
					if (is_regular(*it)) {
						images.push_back((*it).string());
					}
				}
			}
		} else if (S_ISREG(s.st_mode)) {
			images.push_back(filepath);
		}
	}
	return images;
}

void CameraCalibrator::StereoCalib(const std::vector<string> &imagelist) {
    if( imagelist.size() % 2 != 0 )
    {
        cout << "Error: the image list contains odd (non-even) number of elements\n";
        return;
    }

    const int maxScale = 2;
    const float squareSize = 2.5f;  //actual chessboard square size in cm

    vector<vector<Point2f> > imagePoints[2];
    vector<vector<Point3f> > objectPoints;
    Size imageSize;

    int i, j, k, nimages = (int)imagelist.size()/2;

    imagePoints[0].resize(nimages);
    imagePoints[1].resize(nimages);
    vector<string> goodImageList;

    for( i = j = 0; i < nimages; i++ )
    {
        for( k = 0; k < 2; k++ )
        {
            const string& filename = imagelist[i*2+k];
            Mat img = imread(filename, 0);

            if(img.empty())
                break;

            if( imageSize == Size() )
                imageSize = img.size();
            else if( img.size() != imageSize )
            {
                cout << "The image " << filename << " has the size different from the first image size. Skipping the pair\n";
                break;
            }
            bool found = false;
            vector<Point2f>& corners = imagePoints[k][j];
            for( int scale = 1; scale <= maxScale; scale++ )
            {
                Mat timg;
                if( scale == 1 )
                    timg = img;
                else
                    resize(img, timg, Size(), scale, scale);
                found = findChessboardCorners(timg, boardSize, corners,
                    CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
                if( found )
                {
                    if( scale > 1 )
                    {
                        Mat cornersMat(corners);
                        cornersMat *= 1./scale;
                    }
                    break;
                }
            }
            if( show )
            {
                cout << filename << endl;
                Mat cimg, cimg1;
                cvtColor(img, cimg, CV_GRAY2BGR);
                drawChessboardCorners(cimg, boardSize, corners, found);
                double sf = 640./MAX(img.rows, img.cols);
                resize(cimg, cimg1, Size(), sf, sf);
                imshow("corners", cimg1);
                char c = (char)waitKey(1);
                if( c == 27 || c == 'q' || c == 'Q' ) //Allow ESC to quit
                    exit(-1);
            }
            else
                putchar('.');
            if( !found )
                break;
            cornerSubPix(img, corners, Size(11,11), Size(-1,-1),
                         TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,
                                      30, 0.01));
        }
        if( k == 2 )
        {
            goodImageList.push_back(imagelist[i*2]);
            goodImageList.push_back(imagelist[i*2+1]);
            j++;
        }
    }
    cout << j << "Pairs have been successfully detected.\n";
    nimages = j;
    if( nimages < 2 )
    {
        cout << "Error: too little pairs to run the calibration\n";
        return;
    }

    imagePoints[0].resize(nimages);
    imagePoints[1].resize(nimages);
    objectPoints.resize(nimages);

    for( i = 0; i < nimages; i++ )
    {
        for( j = 0; j < boardSize.height; j++ )
            for( k = 0; k < boardSize.width; k++ )
                objectPoints[i].push_back(Point3f(j*squareSize, k*squareSize, 0));
    }

    cout << "Running stereo calibration ...\n";

    Mat cameraMatrix[2], distCoeffs[2];
    cameraMatrix[0] = Mat::eye(3, 3, CV_64F);
    cameraMatrix[1] = Mat::eye(3, 3, CV_64F);
    Mat R, T, E, F;

    double rms = stereoCalibrate(objectPoints,imagePoints[0], imagePoints[1],
            cameraMatrix[0], distCoeffs[0],
            cameraMatrix[1], distCoeffs[1],
            imageSize,R,T,E,F,
            CV_CALIB_FIX_ASPECT_RATIO +
            CV_CALIB_ZERO_TANGENT_DIST +
            CV_CALIB_SAME_FOCAL_LENGTH +
            CV_CALIB_RATIONAL_MODEL +
            CV_CALIB_FIX_K3 + CV_CALIB_FIX_K4 + CV_CALIB_FIX_K5,
            TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 100, 1e-5));

    cout << "done with RMS error=" << rms << endl;

    // CALIBRATION QUALITY CHECK
    double err = 0;
    int npoints = 0;
    vector<Vec3f> lines[2];
    for( i = 0; i < nimages; i++ )
    {
        int npt = (int)imagePoints[0][i].size();
        Mat imgpt[2];
        for( k = 0; k < 2; k++ )
        {
            imgpt[k] = Mat(imagePoints[k][i]);
            undistortPoints(imgpt[k], imgpt[k], cameraMatrix[k], distCoeffs[k], Mat(), cameraMatrix[k]);
            computeCorrespondEpilines(imgpt[k], k+1, F, lines[k]);
        }
        for( j = 0; j < npt; j++ )
        {
            double errij = fabs(imagePoints[0][i][j].x*lines[1][j][0] +
                                imagePoints[0][i][j].y*lines[1][j][1] + lines[1][j][2]) +
                           fabs(imagePoints[1][i][j].x*lines[0][j][0] +
                                imagePoints[1][i][j].y*lines[0][j][1] + lines[0][j][2]);
            err += errij;
        }
        npoints += npt;
    }
    cout << "average reprojection err = " <<  err/npoints << endl;

    // save intrinsic parameters
    FileStorage fs(intrinsics.c_str(), CV_STORAGE_WRITE);
    if( fs.isOpened() )
    {
        fs << "M1" << cameraMatrix[0] << "D1" << distCoeffs[0] <<
            "M2" << cameraMatrix[1] << "D2" << distCoeffs[1];
        fs.release();
    }
    else
        cout << "Error: can not save the intrinsic parameters\n";

    Mat R1, R2, P1, P2, Q;
    Rect validRoi[2];

    stereoRectify(cameraMatrix[0], distCoeffs[0],
                  cameraMatrix[1], distCoeffs[1],
                  imageSize, R, T, R1, R2, P1, P2, Q,
                  CALIB_ZERO_DISPARITY, 1, imageSize, &validRoi[0], &validRoi[1]);

    fs.open(extrinsics.c_str(), CV_STORAGE_WRITE);
    if( fs.isOpened() )
    {
        fs << "R" << R << "T" << T << "R1" << R1 << "R2" << R2 << "P1" << P1 << "P2" << P2 << "Q" << Q;
        fs.release();
    }
    else
        cout << "Error: can not save the extrinsics parameters\n";
}

void CameraCalibrator::calibrate(string calibImagesPath, string in, string ex, Size boardS) {
	boardSize = boardS;
	intrinsics = in;
	extrinsics = ex;

    vector<string> images = getImagesFromDir(calibImagesPath);

	StereoCalib(images);

	cout << "Calibration finished..." << endl;
}

void CameraCalibrator::createNewCalibrationImages(string filepath, int camIndex1, int camIndex2) {

    Size image_size = Size(320,240);
    Mat leftFrame, rightFrame, leftResized, rightResized;
    VideoCapture capLeft, capRight;
    int imageID = 1;
    if(!capLeft.open(camIndex1) || !capRight.open(camIndex2)) {
        cout << "could not open camera stream!" << endl;
        exit(1);
    }

    //capLeft.set(CV_CAP_PROP_FRAME_WIDTH,320);
    //capLeft.set(CV_CAP_PROP_FRAME_HEIGHT,240);
    //capRight.set(CV_CAP_PROP_FRAME_WIDTH,320);
    //capRight.set(CV_CAP_PROP_FRAME_HEIGHT,240);

    for(;;) {
        if (!capLeft.read(leftFrame) || !capRight.read(rightFrame)) {
            cout << "could not read frames!!" << endl;
            exit(1);
        }

        resize(leftFrame, leftResized, image_size);
        resize(rightFrame, rightResized, image_size);

        if(show) {
            imshow("left", leftResized);
            imshow("right", rightResized);
        }

        if(waitKey(30) >= 0) {
            cout << "taking photo!" << endl;
            path p(filepath);

            if(is_directory(p)) {
                String imageIDString;
                if(imageID / 10 == 0) {
                    imageIDString = "0"+to_string(imageID);
                } else {
                    imageIDString = to_string(imageID);
                }

                imwrite(p.string()+imageIDString+"_l.bmp", leftFrame);
                imwrite(p.string()+imageIDString+"_r.bmp", rightFrame);
                imageID++;
                if(imageID > 99) {
                    break;
                }
            }
            else {
                cout << "Path '" + filepath + "' is not a directory!" << endl;
            }
        }
    }
}
