
#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <opencv2/opencv.hpp>

#define DEFAULT_PROCESS_FRAMES  5

struct PixelMap {
public:
    cv::Vec3b   col_pix;
    uchar       gray_pix;
    
    friend bool operator < (const PixelMap& m1,const PixelMap& m2) {
        return (m1.gray_pix < m2.gray_pix);
    }
    
};

typedef std::vector<PixelMap>   PixelList;
typedef std::list<cv::Mat>      FrameList;

cv::Mat calc_temporal_median(FrameList &pFrame_list) {
    int frame_num = pFrame_list.size();
    std::vector<cv::Mat> gray_frames;
    std::vector<cv::MatConstIterator_<cv::Vec3b> > bgr_it;
    cv::MatConstIterator_<cv::Vec3b> base_it,base_end;
    std::vector<cv::MatConstIterator_<uchar> >    gray_it;
    cv::Mat dst(pFrame_list.begin()->rows, pFrame_list.begin()->cols, CV_8UC3);
    cv::MatIterator_<cv::Vec3b> dst_it = dst.begin<cv::Vec3b>();
    
    cv::Mat gray_frame,gray_frame_clone;
    for(FrameList::const_iterator it = pFrame_list.begin();it != pFrame_list.end();++it) {
        if(it == pFrame_list.begin()) {
            base_it     = it->begin<cv::Vec3b>();
            base_end    = it->end<cv::Vec3b>();
        }
        
        cv::cvtColor(*it, gray_frame, cv::COLOR_BGR2GRAY);
        gray_frame_clone = gray_frame.clone();
        gray_frames.push_back(gray_frame_clone);
        bgr_it.push_back    (it->begin<cv::Vec3b>());
        gray_it.push_back   (gray_frame_clone.begin<uchar>());
    }
    
    for ( ; base_it != base_end; base_it++) {
        PixelList pixel_list;
        pixel_list.clear();
        
        for(int i = 0;i < frame_num;++i) {
            PixelMap pixel_map;
            pixel_map.col_pix = *(bgr_it[i]);
            pixel_map.gray_pix  = *(gray_it[i]);
            pixel_list.push_back(pixel_map);
            (bgr_it[i])++;
            (gray_it[i])++;
        }
        
        std::sort(pixel_list.begin(),pixel_list.end());
        *dst_it = (pixel_list[frame_num / 2]).col_pix;
        dst_it++;
    }
    
    return dst;
}

int main(int argc, char *argv[] ) {
    if (argc < 3) {
        std::cout << "Usage: MedianVideo input_movie output_movie [number of frames]" << std::endl;
        return -1;
    }
    std::string input_movie(argv[1]);
    std::string output_movie(argv[2]);
    int median_frames;
    if(argc >= 4)
        median_frames = std::atoi(argv[3]);
    else
        median_frames = DEFAULT_PROCESS_FRAMES;

    if (median_frames <= 0) median_frames = DEFAULT_PROCESS_FRAMES;

    cv::VideoCapture video_capture;

    if ( ! video_capture.open(input_movie) ) {
        std::cout << "Could not open " << input_movie << std::endl;
        return -1;
    }
    
    cv::VideoWriter video_writer;

    //int format = cv::VideoWriter::fourcc('D','I','V','X');
    //int format = cv::VideoWriter::fourcc('X','V','I','D');
    int format = cv::VideoWriter::fourcc('H','2','6','4');
    
    cv::Size frame_size = 
        cv::Size((int)video_capture.get(CV_CAP_PROP_FRAME_WIDTH),
            (int)video_capture.get(CV_CAP_PROP_FRAME_HEIGHT));
            
    int fps = video_capture.get(CV_CAP_PROP_FPS);

    video_writer.open(output_movie, format, fps, frame_size, true);

    if ( ! video_writer.isOpened() ) { 
        std::cout << "VideoWriter could not open." << std::endl;
        return -1;
    }

    int frame_count = 0;
    const int frame_total = video_capture.get(CV_CAP_PROP_FRAME_COUNT);
    const int width = std::to_string(frame_total).size();

    std::cout << "Input movie: " << input_movie << std::endl;
    std::cout  << "Output movie: " << output_movie << std::endl;
    std::cout << "Nummber of frames to median: " << median_frames << std::endl << std::endl;

    FrameList frame_list;
    
    while (true)
    {
        std::cout << "\rframe " <<  ++frame_count << "/" << frame_total << "    " << std::flush;

        cv::Mat frame;
        video_capture >> frame;

        if (! frame.empty()) {
            frame_list.push_back(frame.clone());
            if(frame_list.size() > median_frames) {
                frame_list.pop_front();
            }
            
            if(frame_list.size()) {
                video_writer << calc_temporal_median(frame_list);
            }
        }
        else
            break;
    }
    std::cout << "Done!" << std::endl;

    return 0;
}

