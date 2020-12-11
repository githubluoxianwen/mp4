#include "libmp4.h"

void Mp4WriteData(void *data,unsigned int size,void *userParams)
{
    if (data == NULL || size < 0 || userParams == NULL)
        return;

	FILE *fp = (FILE *)userParams;
    fwrite((char *)data,1,size,fp);  
 
    return;
}

void main()
{
	FILE *fp = NULL;
	int ret = 0;
	int mp4Handle = 0;
	char *sps = NULL;
	uint32 spsNum = 0;
	char *pps = NULL;
	uint32 ppsNum = 0;
    MP4_ATTR_T attr = {0};
	
	char streamData[] = {0x00,0x00,0x00,0x01,0x67,0x23,0x46,0x64,0x63,0x98,0x34,0x63,0x34,0x66,
				   0x00,0x00,0x00,0x01,0x68,0x32,0x64,0x46,0x36,0x89,0x00,0x00,0x00,0x01,
				   0x65,0x23,0x46,0x64,0x63,0x98,0x34,0x63,0x34,0x66,0x43,0x36,0x43,0x66,
				   0x67,0x23,0x46,0x64,0x63,0x98,0x34,0x63,0x34,0x66,0x43,0x36,0x43,0x66};	
	int dataSize = sizeof(streamData);
	
	fp = fopen("mpeg.mp4","wb+");
	
    mp4Handle = MP4Package_CreateMP4Object();
	if (mp4Handle < 0)
	{
		printf("create object failed!\n");
		goto ERROR_RETURN;
	}

	ret = MP4Package_H264ReadEncodeInfo(mp4Handle,streamData,dataSize,&sps,&spsNum,&pps,&ppsNum);	
	if (0 != ret)
	{
		printf("read h264 failed!\n");
		goto ERROR_RETURN;
	}
	
    memset(&attr,0,sizeof(MP4_ATTR_T));
    attr.fps = 25;//ѡ��
    attr.druation = 60*1000;//60000ms 1min ѡ��
    attr.timeScan = 1000;//1000ms ������
    attr.width = 1028 << 16;//������
    attr.height = 1290 << 16;//������
    attr.volume = 1 << 8;//������
    attr.rate = 1 << 16;//������
	attr.startTime = 0;
	attr.stopTime = 0;

	attr.spsTable = sps;
	attr.spsNum = spsNum;
	attr.ppsTable = pps;
	attr.ppsNum = ppsNum;
	attr.type = 0;//0-ʵʱ����       1-���ļ�
//	��ʹ��ʵʱ������ʱ��,������д
	attr.sampleSize = 3*1024*1024;//������
    attr.fileSize = 3*1024*1024 + 1024*10;
    MP4Package_SetMP4Attr(mp4Handle,&attr);

	ret = MP4Package_WriteHead(mp4Handle,streamData,sizeof(streamData),Mp4WriteData,(void *)fp);
	if (0 != ret)
	{
		printf("write Head failed!\n");
		goto ERROR_RETURN;
	}

    while (1)
    {
		/*
		��ȡ��������
		*/
			
	  	ret = MP4Package_WriteSample(mp4Handle,streamData,sizeof(streamData),Mp4WriteData,(void *)fp);
	  	if (ret != 0)
	  	{
	     	/*
	     	�������ͷ�
	     	*/
			break;
	  	} 	  
		break;
		/*
		�������ͷ�
		*/
  	}

  	MP4Package_WriteSample(mp4Handle,&mp4Handle,0,Mp4WriteData,(void *)fp);
	/*
	���ر�
	*/
  	MP4Package_WriteMetadata(mp4Handle,Mp4WriteData,(void *)fp);
 	MP4Package_DestoryMP4Object(mp4Handle);

	fclose(fp);

	return;
	
ERROR_RETURN:	
	MP4Package_DestoryMP4Object(mp4Handle);
	printf("some error return\n");
	
	return;
}