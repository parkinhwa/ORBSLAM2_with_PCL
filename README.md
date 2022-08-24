# 3D mapping based on ORB SLAM by using drones

## 프로젝트 내용 설명
드론으로 실내를 촬영하고 그 영상을 토대로 객체를 인식해 표시하고 지도를 만드는 지도 생성기입니다.


## 프로젝트 역할 분배

![image](https://user-images.githubusercontent.com/65644486/139809559-7f0ca238-c5ab-41d2-9a5f-e5fe7bb1c690.png)


## 수행내용

### 1. 객체 인식

![image](https://user-images.githubusercontent.com/65644486/139801318-879ab55a-f7d5-4935-9a89-3288a5ef42fb.png)

그림 1. mAP 성능표


mAP(mean average percision)은 높을수록 알고리즘 성능이 높은 것을 알 수 있다.
※ mAP@0.5 : IoU≥0.5인 것들의 precision을 계산한 후 그 평균을 낸 것 => 0.99
※ mAP@0.5:0.9 : IoU가 0.5~0.95인 것들의 precision을 계산한 후 평균을 낸 것=> 0.68
※ IoU(Intersection over Union) : 보통 두 가지 물체의 위치(Bounding Box)가 얼마나 일치하는지를 수학적으로 나타내는 지표이다.


![image](https://user-images.githubusercontent.com/65644486/139801304-2a22ad80-690b-4eeb-974f-b5d9065c8eec.png)

그림 2 loss 표


loss (손실률)은 낮을수록 높은 성능을 지닌다.
※ Box loss : 예측Box의 loss => 0.022
※ Objectness loss : 예측Box가 Object일 확률 * 정답box와의 IOU값에 대한 loss=> 0.0003
※ Classification loss : 정답box가 겹치는 IOU값에 따른 Anchor Box에 대한 loss=> 0.0036

![image](https://user-images.githubusercontent.com/65644486/139801276-0bc5624c-92c8-40b0-8315-a4595ed50e72.png)

그림 3 yolov5 model mAP

본 캡스톤에서는 mAP(0.5:0.95)의 값이 0.68이 나왔다. 환경이 다른걸 감안했어도 공식문서에 나와있는 44.5보다 높은 값이 나왔고 성능이 우수한 것을 확인할 수 있다. 

![image](https://user-images.githubusercontent.com/65644486/139809799-c3f5cb51-f9d5-4537-a14a-5836725eb2d5.png)

실제로 N4동 6층 복도에서 테스트해본 결과이며 인식률이 60%~90% 성능을 보인다.

중간보고 때는 객체를 잘못 인식하여 그림5와 같이 iou값이 0이 나오는 경우도 있었으나 이를 최종 보고(그림6)때 개선하였다. iou_average는 정답 값의 bounding box와 detect한 bounding box의 겹치는 비율을 뜻하고 평균적으로 정답과 약 80% 일치하는 것을 확인할 수 있었고 Detection rate는 탐지율로 정답을 맞힌 객체 수/detect를 실행한 전체 객체 수를 뜻한다. 약 95%로 높은 탐지율을 보인다. 이는 중간보고 때보다 성능이 높아진 것을 확인할 수 있다.

![image](https://user-images.githubusercontent.com/65644486/139809869-3760e6b9-c3f8-4170-b8dc-bcf2463f7ac1.png)

![image](https://user-images.githubusercontent.com/65644486/139809975-490c9ef6-6c4a-4076-99db-5d72b54424c0.png)

그림7를 보면 본 프로젝트와 같은 환경(Tesla T4 GPU)에서 테스트한 inference time을 확인할 수 있다. 그림에서 yolov5m의 inference time은 20ms정도이며 실제 실행해본 결과 그림 8와 같이 inference time이 평균 14.05ms로 위 표보다 높은 성능을 보였고 목표 요구사항을 충족하였다.

관련 코드 - https://drive.google.com/drive/folders/1rs-JgHak2jNzhdjyJF9mRevrhs4lLOA-?usp=sharing

### 2. 지도 개선

![image](https://user-images.githubusercontent.com/65644486/139804296-6b066080-eeb5-4c8f-a625-59d00d2a8094.png)

realsense d435i 카메라를 사용하여 depth 이미지 추출하고 ORB SLAM 기반으로 지도를 만들었지만 만들어진 지도가 시인성이 떨어지는 문제가 발생하였다.
이를 해결하기 위해 고안한 방법은 다음과 같다.

### 2.1. depth 밝기 조정

![image](https://user-images.githubusercontent.com/65644486/139804393-52ce7816-fbbd-475b-8e6e-6e1c95c92812.png)

opendataset의 depth 이미지 밝기가 우리와 비교해서 확연히 밝았다. 이 문제가 왜 발생하는지 이유를 찾아보니 지도를 제작하기 위해 realsense 카메라로 찍은 bag 영상 파일을 depth 이미지와 color 이미지로 나누는데 이 과정에서 발생하는 문제라는 것을 알아냈다. 우리의 depth 영상은 16bit를 띄는데 opencv는 8bit까지 지원하여 어둡게 보이는 것이라는 알게되어 이를 임의로 16bit로 바꿔주었으나 지도가 오히려 전보다 더 잘 구현되지 못하는 결과를 초래했다. 이로 인해 depth의 밝기는 지도 개선의 영향을 끼치지 않는다는 것을 확인하였다.

![image](https://user-images.githubusercontent.com/65644486/139804121-9029193d-946a-46b4-8107-a0517f744a8a.png)

그림 14 개선된 depth 이미지로 생성된 지도

### 2.2. noise 제거

지도를 생성할 때 오류로 발생된 point(noise)들을 Statistical- Outlier-Removal filter로 제거하였다. Statistical- Outlier-Removal filter는 이웃 근접 point들과 평균 거리 정보를 계산하여 분포를 만들고 그 만든 분포가 gaussian distribution 따른다는 가정하에 나머지는 noise로 간주, 제거하는 방식이다.

![image](https://user-images.githubusercontent.com/65644486/139804900-9533ac23-67f1-4040-a9de-f80c2982bc37.png)

그림 15 noise 제거 전 후

### 2.3. 천장 제거

천장을 제거하여 위에서 내부가 보이도록 하여 지도를 보기좋게 하려했다. 바닥 제거에 많이 쓰이는 RANSAC 알고리즘을 사용하였고 z값을 조정해 천장을 제거 하려했으나 만들어진 지도의 z축이 예상과 다르게 일정하지 않고 비스듬한 지도가 제작되어 알고리즘을 적용해도 변화가 없었다.

![image](https://user-images.githubusercontent.com/65644486/139805271-00edd07f-aaa4-4067-b764-8c2ebab08680.png)

그림 16 천장제거 전 후

### 3. 인터페이스  

촬영한 영상만 있으면 사용자가 지도를 만들 수 있도록 지도 제작 과정을 자동화하는 인터페이스를 제작했다.

![image](https://user-images.githubusercontent.com/65644486/139805495-5608e236-ba91-430a-911c-79ea619ffa5a.png)

그림 17 지도를 제작, 확인하는 인터페이스

드론으로 촬영한 .bag파일 영상을 가져와서 지도를 제작하기를 누르면 color 이미지와 depth 이미지를 나누고 그 이미지 토대로 지도를 제작한다. 제작한 지도는 ORBSLAM2_with_PCL 폴더에 저장되며 저장된 지도를 불러와 확인할 수 있다. 버튼을 누르면 sh 파일을 작동시켜 command 창에서 쳐야하는 모든 명령어를 수행하게 만들었고 버튼을 누르고 지도제작 sh가 작동하면서 인터페이스가 중간에 중단하는 문제가 발생하였으나 스레드를 사용하여 해결하였다.

### 4. 라즈베리파이 세팅
드론의 부착할 라즈베리파이는 realsense camera로 영상을 촬영해야하고 그 영상을 컴퓨터가 받아서 지도를 제작한다.
intel에서 제공하는 realsense SDK가 설치된 리눅스가 있어 그것을 SD카드에 포팅한 후 라즈베리파이에 넣어주었다.

관련 사이트 - https://dev.intelrealsense.com/docs/open-source-ethernet-networking-for-intel-realsense-depth-cameras#25-first-boot
