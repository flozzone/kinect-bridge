/**
 * This file is part of the nestk library.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Nicolas Burrus <nicolas.burrus@uc3m.es>, (C) 2010
 */

#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <ntk/ntk.h>
#include <ntk/utils/debug.h>
#include <ntk/camera/openni_grabber.h>
#include <ntk/gesture/body_event.h>

#include <QApplication>
#include <QDir>
#include <QMutex>

#include "cvmat_serialization.h"

using namespace cv;
using namespace ntk;

namespace opt
{
ntk::arg<bool> high_resolution("--highres", "High resolution color image.", 0);
ntk::arg<int> kinect_id("--kinect-id", "Kinect id", 0);
}

int main(int argc, char **argv)
{
    // Parse command line options.
    arg_base::set_help_option("-h");
    arg_parse(argc, argv);

    // Set debug level to 1.
    ntk::ntk_debug_level = 1;

    // Set current directory to application directory.
    // This is to find Nite config in config/ directory.
    QApplication app (argc, argv);
    QDir::setCurrent(QApplication::applicationDirPath());

    // Declare the global OpenNI driver. Only one can be instantiated in a program.
    OpenniDriver ni_driver;

    // Declare the frame grabber.
    OpenniGrabber grabber(ni_driver, opt::kinect_id());

    // High resolution 1280x1024 RGB Image.
    if (opt::high_resolution())
        grabber.setHighRgbResolution(true);

    // Start the grabber.
    grabber.connectToDevice();
    grabber.start();

    // Holder for the current image.
    RGBDImage image;

    // Image post processor. Compute mappings when RGB resolution is 1280x1024.
    OpenniRGBDProcessor post_processor;

    namedWindow("depth");
    namedWindow("color");

    char last_c = 0;
    while (true && (last_c != 27))
    {
	// Wait for a new frame, get a local copy and postprocess it.
        grabber.waitForNextFrame();
        grabber.copyImageTo(image);
	post_processor.processImage(image);

        cv::Mat depth_tx;
        cv::Mat color_tx;

	{
	    // Prepare the depth view, mapped onto rgb frame.
	    cv::Mat1b debug_depth_img = normalize_toMat1b(image.mappedDepth());
	    depth_tx = debug_depth_img;

	    // Prepare the color view with skeleton and handpoint.
	    Mat3b debug_color_img;
	    image.rgb().copyTo(debug_color_img);
	    color_tx = debug_color_img;
	}

	std::cout << "rows:" << color_tx.rows << std::endl;
	std::cout << "cols:" << color_tx.cols << std::endl;



	std::ofstream ofs("matrices_depth.bin", std::ios::out | std::ios::binary);

        { // use scope to ensure archive goes out of scope before stream

	    boost::archive::text_oarchive oa(ofs);
	    oa << color_tx; //<< color_tx;
        }

        ofs.close();


	imshow("depth", depth_tx);
	imshow("color", color_tx);

	IplImage iplImage;

	//cvConvertImage(&color_tx, &iplImage);
	iplImage = color_tx;


	//if(!cvSaveImage("test.tpl", &iplImage)) printf("Could not save: test.tpl\n");

        last_c = (cv::waitKey(10) & 0xff);
    }

    grabber.stop();
}
