
with open('/home/skennetho/ORBSLAM2_with_PCL/dataset/rgbd_hall_6f/association.txt', 'r') as association:
    ac = association.readlines()
    #rgb_timestamp filename depth_timestamp filename

with open('mid.txt', 'r') as _mid:
    mid = _mid.readlines()



def makeYoloResultTextfile(mid):
    i=0
    save_list = []
    for ac_line in ac:
        # print(ac_line)
        for j in range(i, len(mid), 2):
            print("mid = ", mid[j].split(' ')[0], ' ac=', ac_line.split(' ')[1].split('/')[1])

            if mid[j].split(' ')[0] == ac_line.split(' ')[1].split('/')[1]: #파일의 타임스탬프 value가 same한지 확인
                save_list.append(mid[j].split(' ')[0] + ' ' + ac_line.split(' ')[0] + "\n")
                save_list.append(mid[j+1])
                i += 2
                break

    file = open('mid_pose.txt', 'w')

    for save in save_list:
        file.write(save)

    file.close()

makeYoloResultTextfile(mid)
