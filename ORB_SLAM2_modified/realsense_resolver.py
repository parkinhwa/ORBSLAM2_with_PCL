import argparse
import pyrealsense2 as rs
import numpy as np
import cv2
import os
import time

def main():
    prev_time = 0
    FPS = 10

    directory = args.input[:-4]
    print(args.input,'to',directory)
    
    if not os.path.exists(directory):
        os.mkdir(directory)
        os.mkdir(directory+"/depth")
        os.mkdir(directory+"/rgb")
    try:
        pipeline = rs.pipeline()
        config = rs.config()
        
        rs.config.enable_device_from_file(config, args.input, repeat_playback=False)
        config.enable_stream(rs.stream.depth, rs.format.z16, 30)
        config.enable_stream(rs.stream.color, rs.format.rgb8, 30)

        pipeline.start(config)
        i=0
        
        align_to = rs.stream.color
        align = rs.align(align_to)

        file = open(directory+'/association.txt',mode ='wt')
        rgb_file = open(directory+'/rgb.txt',mode ='wt')
        depth_file = open(directory+'/depth.txt',mode ='wt')
        
        while True:
            current_time = time.time() - prev_time
            if current_time > 1./ FPS :
                prev_time = time.time()
                

                frames = pipeline.wait_for_frames()

                #depth_frame = frames.get_depth_frame()
                aligned_frames = align.process(frames)
                aligned_depth_frame = aligned_frames.get_depth_frame()
                color_frame= frames.get_color_frame()

                if not aligned_depth_frame or not color_frame:
                    print("problem here")
                    raise RuntimeError("Could not acquire depth or color frames.")

                depth_image = np.asanyarray(aligned_depth_frame.get_data())
                #image_8bit = cv2.convertScaleAbs(depth_image, alpha=0.03)
                #depth_colormap = cv2.applyColorMap(cv2.convertScaleAbs(depth_image, alpha=0.03), cv2.COLORMAP_JET)
                color_image= np.asanyarray(color_frame.get_data())
                color_image =cv2.cvtColor(color_image,cv2.COLOR_BGR2RGB)
                
                
                cv2.imshow('depth Example',depth_image)
                
                filename = str(i).zfill(6) + ".png"
                cv2.imwrite(directory + "/depth/" + filename, depth_image)
                cv2.imwrite(directory + "/rgb/" + filename, color_image)
                
                num =str('{:.2f}'.format(i*0.2)).zfill(6)
                file.write(num+ " rgb/"+filename+" "+num+" depth/"+filename+"\n")
                
                rgb_file.write(num+ " rgb/"+ filename+"\n")
                depth_file.write(num+ " depth/"+filename+"\n")

                print("Saved frame:"+filename)
                
                i += 1
   
    finally:
        pass
        



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", type=str, help="Bag file to read")

    args = parser.parse_args()

    main()