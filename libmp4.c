/*****************************************************************************************************************\
 * �汾��V1.0.0    
 * ���ߣ�luoxianwen 
 * ʱ�䣺2018-02-09
 * ��������������Ƶ����װMP4�ļ���ʽ
 * ����Ԥ��Ŀ�꣺1.��װ�ɿ����ʽ
 *	             2.֧�ֶ�·ͬʱ��װ
 *	             3.֧������Ƶͳһ��װ
 *	             4.֧��H254��H256��������������ʽ��װ
 *			     5.���������ṩAPIʹ��˵���ĵ�������dome�����ߺʹ�ӡ��MP4�ļ�Э���ĵ�������м��׵Ĳ��������Բ���
/*******************************************************************************************************************/
#include "libmp4.h"

#define MP4_DEBUG  1

#define MP4DBG(fmt,...)  do{printf("[MP4DBG]:[%s:%d] ",__FUNCTION__,__LINE__);printf(fmt,##__VA_ARGS__);}while(0);

#define H264_HEADER_START  0x01000000
#define H264_HEADER_SPS    0x07
#define H264_HEADER_PPS    0x08
#define H264_HEADER_IDR_SLICE  0x05
#define H264_HEADER_SLICE  0x01
#define H264_HEADER__SEI   0x06
#define IS_NEED_TRANSLATE(x,i)  (x[i] == 0x00 && x[i+1] == 0x00 && x[i+2] == 0x03)
#define IS_HEADER_START(x,i)  (x[i] == 0x00 && x[i+1] == 0x00 && x[i+2] == 0x00 && x[i+3] == 0x01)
#define MP4_MDAT_SIZE  2 * 1000 * 1000 - 48//28 * 1000 * 1000 //28M ��ȥ�ٲ�

static char g_matrix[36] = {0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, 
                            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00};

static char g_pre_defined[24] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

static char g_compressionName[42] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0xFF,0xFF};


void Mp4WriteData(void *data,unsigned int size,void *userParams)
{
    if (data == NULL || size < 0 || userParams == NULL)
        return;

	FILE *fp = (FILE *)userParams;
    fwrite((char *)data,1,size,fp);  
 
    return;
}

#if 0
void main()
{
	int fd = 0;
	int ret = 0;
	char *data = NULL;
	int dataSize = 0;
	int mp4Handle = 0;
	char *sps = NULL;
	uint32 spsNum = 0;
	char *pps = NULL;
	uint32 ppsNum = 0;
    MP4_ATTR_T attr = {0};
	
    mp4Handle = MP4Package_CreateMP4Object();
	if (mp4Handle < 0)
	{
		goto ERROR_RETURN:
	}

	ret = MP4Package_H264ReadEncodeInfo(mp4Handle,data,dataSize,&sps,&spsNum,&pps,&ppsNum);	
	if (0 != ret)
	{
		goto ERROR_RETURN:
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
	attr.spsNum = 0;
	attr.ppsTable = pps;
	attr.ppsNum = 0;
	attr.type = 0;//0-ʵʱ����       1-���ļ�
//	��ʹ��ʵʱ������ʱ��,������д
	attr.sampleSize = 30*1024*1024;//������
    attr.fileSize = 30*1024*1024 + 1024*10;
    MP4Package_SetMP4Attr(mp4Handle,&attr);

	ret = MP4Package_WriteHead(mp4Handle,data,dataSize,Mp4WriteData,(void *)&fd);
	if (0 != ret)
	{
		goto ERROR_RETURN:
	}

    while (1)
    {
		/*
		��ȡ��������
		*/
			
	  	ret = MP4Package_WriteSample(mp4Handle,data,dataSize,Mp4WriteData,(void *)&fd);
	  	if (ret != 0)
	  	{
	     	/*
	     	�������ͷ�
	     	*/
			break;
	  	} 	  
		/*
		�������ͷ�
		*/
  	}

  	MP4Package_WriteSample(mp4Handle,&mp4Handle,0,Mp4WriteData,(void *)&fd);
	/*
	���ر�
	*/
  	MP4Package_WriteMetadata(mp4Handle,Mp4WriteData,(void *)&fd);
 	MP4Package_DestoryMP4Object(mp4Handle);

	

ERROR_RETURN:	
	MP4Package_DestoryMP4Object(mp4Handle);
	printf("some error return\n");
	
	return;
}
#endif

MP4_MANAGER g_mp4Manager = {0};

void PrintBuffer(void *buffer, int len)
{
    unsigned char* p = (unsigned char*)buffer;
    int i;
    
    printf("[yuliang][%s:%d] buffer=%p len=0x%x\n", __FUNCTION__, __LINE__, buffer, len);
    for (i = 0; i < len; i += 16)
    {
        int j;
        printf("0x%04x:", i);
        for (j = 0; j < 16 && i + j < len; j++)
        {
            printf(" %02x", p[i + j]);
        }
        if (j < 16)
        {
            for ( ; j < 16; j++)
            {
                printf("   ");
            }
        }
        printf(" | ");
        for (j = 0; j < 16 && i + j < len; j++)
        {
            char c = p[i + j];
            printf("%c", c >= 0x20 && c <= 0x7f ? c : '.');
        }
        printf("\n");
    }
    printf("\n");
}


static uint32 IsIFrame(char *data,uint32 size)
{
	int i = 0;

	for (i = 0; i < size - 4; i++)
	{
		if (IS_HEADER_START(data, i))
		{
			if ((data[i+4] & 0x1f) == H264_HEADER_IDR_SLICE)
				return 1;
		}
	}
    return 0;
}

static int IsVideo(char *data,uint32 size)
{	
int i = 0;

	for (i = 0; i < size - 4; i++)
	{
	    if (IS_HEADER_START(data, i))
	    {	
	    	if ((data[i+4] & 0x1f) == H264_HEADER_IDR_SLICE
				|| (data[4] & 0x1f) == H264_HEADER_SLICE)
	        	return 1;
	    }  
	}

    return 0;
}


static uint32 MP4Package_H264ToAVC1(char *h264,char *avc,int size)
{
	int i = 0,j = 0,k = 0;
	int avcCount = 5;
	char *buf = h264;

    for (i = 0; i < size - 4; i++)
    {
        if (IS_HEADER_START(buf,i))
        {
            if (((buf[i+4] & 0x1f) == H264_HEADER_IDR_SLICE) 
                || ((buf[i+4] & 0x1f) == H264_HEADER_SLICE))
            {
                for (j = i + 5; j < size; j++)
                {
                    if (IS_NEED_TRANSLATE(buf,j))
                    {
                        avc[avcCount++] = 0x00;
                        avc[avcCount++] = 0x00;
                        //avc[avcCount++] = buf[j+3];
                        //j += 3;
                        avc[avcCount++] = 0x03;
                        j += 2;
                    }
                    else
                    {
                        avc[avcCount++] = buf[j];
                    }
                }
				
				for (k = 0; k < size - j; k++)
					avc[avcCount++] = buf[j++];
                //avc[avcCount++] = buf[j++];
                //avc[avcCount++] = buf[j++];
                //avc[avcCount++] = buf[j++];
                break;
            }
        }
    }

	if (i == size - 4)
	{
        return -1;
	}
	else
	{
	    *((unsigned int *)avc) = htonl(avcCount-4);
	    avc[4] = buf[i+4];
        return avcCount;
	}

	return 0 ;    
}


static uint32 MP4_FreeObject(MP4_OBJECT_T *object)
{
	uint32 ret = 0;
	
	if (object == NULL)	
	{
		MP4DBG("object is NULL\n");
		return -1;
	}

	if (object->mp4Attr.ppsTable)
		free(object->mp4Attr.ppsTable);
	object->mp4Attr.ppsTable = NULL;

	if (object->mp4Attr.spsTable)
		free(object->mp4Attr.spsTable);
	object->mp4Attr.spsTable = NULL;

	if (object->sampleInfo.syncList)
		free(object->sampleInfo.syncList);
	object->sampleInfo.syncList = NULL;

	if (object->sampleInfo.chunkList)
		free(object->sampleInfo.chunkList);
	object->sampleInfo.chunkList = NULL;

	if (object->sampleInfo.sizeList)
		free(object->sampleInfo.sizeList);
	object->sampleInfo.sizeList = NULL;

	if (object->sampleInfo.chunkOffsetList)
		free(object->sampleInfo.chunkOffsetList);
	object->sampleInfo.chunkOffsetList = NULL;

	MP4_SAMPLE_NODE *freeNode = NULL;
	MP4_SAMPLE_NODE *nextNode = NULL;
	if (object->sampleInfo.sampleNodeListHead)
	{
		freeNode = object->sampleInfo.sampleNodeListHead;
		nextNode = object->sampleInfo.sampleNodeListHead->sampleNodeListNext;
		while (freeNode)
		{
			free(freeNode);
			freeNode = nextNode;
			if (nextNode)
				nextNode = nextNode->sampleNodeListNext;
		}
	}
	object->sampleInfo.sampleNodeListHead = NULL;

	free(object);

	return ret;
}


static uint32 MP4_DeleteObject(uint32 srcId)
{
	uint32 i = 0;
	uint32 ret = 0;
	MP4_OBJECT_T *object = NULL;

	if (g_mp4Manager.objectCount <= 0)
	{
		MP4DBG("no object\n");
		return 0;
	}

	object = g_mp4Manager.objectsListHead;
	for (i = 0; i < g_mp4Manager.objectCount; i++)
	{
		if (object->objectId == srcId)
		{
			if (object->objectPrev && object->objectNext)
			{
				object->objectPrev->objectNext = object->objectNext;
				object->objectNext->objectPrev = object->objectPrev;
			}
			else if (object->objectPrev && object->objectNext == NULL)
			{
				object->objectPrev->objectNext = NULL;
				g_mp4Manager.objectsListTail = object->objectPrev;
			}
			else if (object->objectPrev == NULL && object->objectNext)
			{
				object->objectNext->objectPrev = NULL;
				g_mp4Manager.objectsListHead = object->objectNext;
			}
			else
			{
				g_mp4Manager.objectsListTail = NULL;
				g_mp4Manager.objectsListHead = NULL;
			}
			break;
		}
		object = object->objectNext;
	}
	
	return ret;
}

static int CheckObject(uint32 handle)
{
	uint32 idx = 0;
	MP4_OBJECT_T *object = NULL;
	
	idx = ((handle >> 8) - 1) & 0xff;
	if (idx < 0 || idx >= MP4_MAX_OBJECT)
	{
		MP4DBG("handle error!\n");
		return -1;
	}

	pthread_mutex_lock(&g_mp4Manager.objectMutex);
	object = g_mp4Manager.objects[idx];
	if (object == NULL)
	{
		MP4DBG("handle error!\n");
		pthread_mutex_unlock(&g_mp4Manager.objectMutex);
		return -1;
	}
	pthread_mutex_unlock(&g_mp4Manager.objectMutex);

	return idx;
}

static int MP4Package_FillSampleNode(MP4_OBJECT_T *object,void *data,int size,int bIframe,MP4PACKAGE_WRITESAMPLE writeCallBack,void *userParams)
{
    if (writeCallBack == NULL)
        return -1;

    uint32 avcSize = 0;
    char *avcBuf = NULL;
    MP4_SAMPLE_NODE *pNode = NULL;

    avcBuf = (char *)malloc(size);
    if (avcBuf == NULL)
    {
        MP4DBG("malloc failed!\n");
        return -1;
    }
    memset(avcBuf,0,size);
    avcSize = MP4Package_H264ToAVC1(data,avcBuf,size);
    if (avcSize <= 0)
    {
        MP4DBG("h264 to avc1 failed!\n");
        return -1;
    }
    
    writeCallBack(avcBuf,avcSize,userParams);
    free(avcBuf);
    avcBuf = NULL;
        
    pNode = (MP4_SAMPLE_NODE *)malloc(sizeof(MP4_SAMPLE_NODE));
    if (pNode == NULL)
    {
        MP4DBG("malloc failed!\n");
        return -1;
    }
    memset(pNode,0,sizeof(MP4_SAMPLE_NODE));
    
    //ÿ����I֡���һ��chunck
    if (bIframe && object->sampleInfo.IframeNum%2 == 0)
    {
        pNode->chunkId = ++object->sampleInfo.chunkCount;
        object->sampleInfo.chunkOffset = object->sampleInfo.offset;
    }
    else
    {
        pNode->chunkId = object->sampleInfo.chunkCount;
    }
    
    if (bIframe)
    {
        pNode->IframeId = ++object->sampleInfo.IframeNum;
    }

    pNode->trackId = 1;   
    pNode->delta = 1;
    pNode->bIframe = bIframe;
    pNode->offset = object->sampleInfo.offset;
    pNode->size = avcSize;
    pNode->sampleId = ++object->sampleInfo.sampleCount;
    pNode->chunkOffset = object->sampleInfo.chunkOffset;
	pNode->sampleNodeListPrev = NULL;
	pNode->sampleNodeListNext = object->sampleInfo.sampleNodeListHead;
	object->sampleInfo.sampleNodeListHead->sampleNodeListPrev = pNode;
	
	object->sampleInfo.sampleNodeListHead = pNode;
    object->sampleInfo.offset += avcSize;
    object->sampleInfo.allSampleSize += avcSize;
	
    return 0;
}



int MP4Package_CreateMP4Object()
{
	uint32 i = 0;
	int ret = 0;
	uint32 idx = 0;
	MP4_OBJECT_T *newObject = NULL;
	
	pthread_mutex_lock(&g_mp4Manager.objectMutex);
	if (g_mp4Manager.objectCount >= MP4_MAX_OBJECT)
	{
		MP4DBG("too match object allObjectCount=%d!\n",g_mp4Manager.objectCount); 			
		pthread_mutex_unlock(&g_mp4Manager.objectMutex);		
		return -1;
	}

	for (i = 0; i < MP4_MAX_OBJECT; i++)
	{
		if (g_mp4Manager.objects[i] == NULL)
		{
			newObject = (MP4_OBJECT_T *)malloc(sizeof(MP4_OBJECT_T));
			if (newObject == NULL)
			{
				pthread_mutex_unlock(&g_mp4Manager.objectMutex);
				MP4DBG("malloc failed!\n");				
				return -1;
			}
			memset((char *)newObject,0,sizeof(MP4_OBJECT_T));
			g_mp4Manager.objects[i] = newObject;
			if (g_mp4Manager.objectsListHead == NULL)
			{
				g_mp4Manager.objectsListHead = newObject;
				g_mp4Manager.objectsListTail = newObject;
				g_mp4Manager.objectsListHead->objectPrev = NULL;
				g_mp4Manager.objectsListHead->objectNext = NULL;
			}
			else
			{
				newObject->objectPrev = NULL;
				newObject->objectNext = g_mp4Manager.objectsListHead;				
				g_mp4Manager.objectsListHead->objectPrev = newObject;
				g_mp4Manager.objectsListHead = newObject;
			}
			idx = i;
			g_mp4Manager.objectCount++;
			break;
		}
	}

	if (i == MP4_MAX_OBJECT)
	{
		MP4DBG("over MP4_MAX_OBJECT=%d!\n",MP4_MAX_OBJECT);
		pthread_mutex_unlock(&g_mp4Manager.objectMutex);
		return -1;
	}
	
	ret = newObject->objectId = ((idx + 1) << 8) | (g_mp4Manager.objectCount & 0xFF); 
	pthread_mutex_unlock(&g_mp4Manager.objectMutex);

	return ret;
}


int MP4Package_DestoryMP4Object(uint32 handle)
{
	int ret = 0;
	uint32 idx = 0;
	MP4_OBJECT_T *deleteObject = NULL;

	idx = ((handle >> 8) - 1) & 0xff;
	if (idx < 0 || idx >= MP4_MAX_OBJECT)
	{
		MP4DBG("handle error!\n");
		return -1;
	}
	
	pthread_mutex_lock(&g_mp4Manager.objectMutex);
	if (g_mp4Manager.objectCount <= 0)
	{
		ret = 0;
		pthread_mutex_unlock(&g_mp4Manager.objectMutex);
		return -1;
	}

	//delete from list
	MP4_DeleteObject(handle);
	deleteObject = g_mp4Manager.objects[idx];
	if (deleteObject != NULL)
	{
		MP4_FreeObject(deleteObject);
		g_mp4Manager.objects[idx] = NULL;
		g_mp4Manager.objectCount--;		
	}

	pthread_mutex_unlock(&g_mp4Manager.objectMutex);
	
	return ret;
}


int MP4Package_SetMP4Attr(uint32 handle, MP4_ATTR_T *attr)
{
	uint32 idx = 0;
	
	if (attr == NULL)
		return -1;

	idx = CheckObject(handle);
	if (idx < 0)
	{
		MP4DBG("check object failed!\n");
		return -1;
	}
	
	pthread_mutex_lock(&g_mp4Manager.objectMutex);
	g_mp4Manager.objects[idx]->mp4Attr = *attr;
	g_mp4Manager.objects[idx]->sampleInfo.mdiaSize = attr->sampleSize + 8;
    g_mp4Manager.objects[idx]->fileHeader.ftyp.boxHeader.size = htonl(sizeof(MP4_FtypInfo));
    g_mp4Manager.objects[idx]->fileHeader.ftyp.boxHeader.type = htonl(0X66747970); //{'f','t','y','p'}
    g_mp4Manager.objects[idx]->fileHeader.ftyp.majorBrand = htonl(0X69736F6D);     //{'i','s','o','m'}
    g_mp4Manager.objects[idx]->fileHeader.ftyp.minorVersion = htonl(0X00000200);
    g_mp4Manager.objects[idx]->fileHeader.ftyp.compatibleBrands[0] = htonl(0X69736F6D);//{'i','s','o','m'}
    g_mp4Manager.objects[idx]->fileHeader.ftyp.compatibleBrands[1] = htonl(0X69736F32);//{'i','s','o','2'}
    g_mp4Manager.objects[idx]->fileHeader.ftyp.compatibleBrands[2] = htonl(0X61766331);//{'a','v','c','1'}
    g_mp4Manager.objects[idx]->fileHeader.ftyp.compatibleBrands[3] = htonl(0X6D703431);//{'m','p','4','1'}
    g_mp4Manager.objects[idx]->fileHeader.free.size = htonl(sizeof(MP4_BoxHeader));
    g_mp4Manager.objects[idx]->fileHeader.free.type = htonl(0X66726565);//{'f','r','e','e'}
    g_mp4Manager.objects[idx]->fileHeader.mdat.size = htonl(attr->sampleSize + 8);
    g_mp4Manager.objects[idx]->fileHeader.mdat.type = htonl(0X6D646174);//{'m','d','a','t'}
	pthread_mutex_unlock(&g_mp4Manager.objectMutex);
	
	return 0;
}


int MP4Package_H264ReadEncodeInfo(uint32 mp4Handle, char *data, uint32 dataSize,char **sps, uint32 *spsNum,char **pps, uint32 *ppsNum)
{
    if (data == NULL || dataSize < 0
		|| sps == NULL || spsNum == NULL
		|| pps == NULL || ppsNum == NULL)
        return -1;

	uint32 id = 0;
    uint32 i = 0,k = 0;
    uint32 spsIdx = 0;
    uint32 ppsIdx = 0;
    char *pp = data;

	id = CheckObject(mp4Handle);
	if (id < 0)
	{
		MP4DBG("check handle error!\n");
		return -1;
	}
	
    if (IS_HEADER_START(pp,0))
    {
        if ((pp[4] & 0x1f) == H264_HEADER_SPS)
        {
            for (i = 4; i < dataSize-4; i++)
            {
                if (IS_HEADER_START(pp,i))
                    break;
            }
            if (i == dataSize)
            {
                MP4DBG("parse h264 header sps failed\n");
                return -1;
            }
            else
            {
                spsIdx = 4;
                *spsNum = i - spsIdx;
            }
        }
        
        if ((pp[i+4] & 0x1f) == H264_HEADER_PPS)
        {
            for (k = i+4; k < dataSize-4; k++)
            {
                if (IS_HEADER_START(pp,k))
                    break;                    
            }
            if (k == dataSize)
            {
                MP4DBG("parse h264 header pps failed\n");
                return -1;
            }
            else
            {
                ppsIdx = *spsNum + spsIdx+4;
                *ppsNum = k - ppsIdx;
            }
        }
    }

    if (*spsNum > 0 && *ppsNum > 0)
    {
        *sps = (char *)malloc(*spsNum);
        *pps = (char *)malloc(*ppsNum);
        memcpy(*sps,data+spsIdx,*spsNum);
        memcpy(*pps,data+ppsIdx,*ppsNum);
    }
    return 0;	
}


int MP4Package_WriteHead(uint32 handle, void *data, uint32 dataSize, MP4PACKAGE_WRITESAMPLE writeCallBack, void *userParams)
{
    if (data == NULL || dataSize <= 0 || writeCallBack == NULL)
    {
        MP4DBG("params error!\n");
        return -1;
    }    

	uint32 idx = 0;
	MP4_OBJECT_T *object = NULL;
	idx = CheckObject(handle);
	if (idx < 0)
	{
		MP4DBG("check failed!\n");
		return -1;
	}
	object = g_mp4Manager.objects[idx];

    uint32 size = 0;
    uint32 avcSize = 0;
    char *avcBuf = NULL;

    writeCallBack((void *)&object->fileHeader,sizeof(MP4_FILE_HEADER),userParams);
	object->sampleInfo.offset += sizeof(MP4_FILE_HEADER);
	
    size = dataSize;    
    avcBuf = (char *)malloc(size);
    if (avcBuf == NULL)
    {
        MP4DBG("malloc failed!\n");
        return -1;
    }
    memset(avcBuf,0,size);
    
    avcSize = MP4Package_H264ToAVC1(data,avcBuf,size);
    if (avcSize <= 0)
    {
        MP4DBG("h264 to avc1 failed!\n");
        return -1;
    }
	
    writeCallBack(avcBuf,avcSize,userParams);
	if (avcBuf)
	    free(avcBuf);
    avcBuf = NULL;
    
    MP4_SAMPLE_NODE *pNode = NULL;
    pNode = (MP4_SAMPLE_NODE *)malloc(sizeof(MP4_SAMPLE_NODE));
    if (pNode == NULL)
    {
        MP4DBG("malloc failed!\n");
        return -1;
    }
    pNode->trackId = 1;
    pNode->chunkId = 1;
    pNode->chunkOffset = object->sampleInfo.offset;
    pNode->delta = 1;
    pNode->bIframe = IsIFrame(data,dataSize);
    pNode->offset = object->sampleInfo.offset;
    pNode->size = avcSize;
    pNode->sampleId = 1;
    pNode->IframeId = 1;
	pNode->sampleNodeListPrev = NULL;
	pNode->sampleNodeListNext = NULL;
	
	object->sampleInfo.sampleNodeListHead = pNode;
	object->sampleInfo.sampleNodeListTail = pNode;
	object->sampleInfo.IframeNum = 1;
	object->sampleInfo.chunkCount = 1;
	object->sampleInfo.sampleCount = 1;
	object->sampleInfo.chunkOffset = object->sampleInfo.offset;
	object->sampleInfo.offset += avcSize;
	object->sampleInfo.allSampleSize += avcSize;
	    
    return 0;
}


int MP4Package_WriteSample(uint32 handle,void *data,uint32 dataSize,MP4PACKAGE_WRITESAMPLE writeCallBack,void *userParams)
{
    if (data == NULL || writeCallBack == NULL)
    {
        MP4DBG("params error!\n");
        return -1;
    }

	int idx = 0;
    int ret = 0;
    int size = 0; 
    int leftSize = 0;
	MP4_OBJECT_T *object = NULL;

	idx = CheckObject(handle);
	if (idx < 0)
	{
		MP4DBG("check handle failed!\n");
		return -1;
	}
	object = g_mp4Manager.objects[idx];
	
    if (dataSize == 0)
    {
        int extData = 0;
        char *p = NULL;
        int size = 500;
        extData = object->sampleInfo.mdiaSize - 8 - object->sampleInfo.allSampleSize;
        if (extData >= size)
        {
            int i = 0;
            p = (char *)malloc(size);
            if (p == NULL)
            {
                MP4DBG("malloc failed!\n");
                return -1;
            }
            memset(p,0,size);
            for (i = 0; i < extData/size; i++)

            {
                writeCallBack(p,size,userParams);
                //usleep(10);
            }
            if (extData%size)
            {
                writeCallBack(p,extData%size,userParams);
            }
            free(p);
        }
        else if (extData > 0)
        {
            p = (char *)malloc(extData);
            if (p == NULL)
            {
                MP4DBG("malloc failed!\n");
                return -1;
            }   
            writeCallBack(p,extData,userParams);
            free(p);
        }          
        return 0;
    }
    
    if (IsVideo(data,dataSize) == 0)
        return 0;
        
//    g_stopTime = header->uiFrameTime;
	object->mp4Attr.stopTime = object->mp4Attr.stopTime;
    
    int bIframe = IsIFrame(data,dataSize);
    size = dataSize;
    leftSize = object->sampleInfo.mdiaSize - 8 - object->sampleInfo.allSampleSize;
    double progress = (double)object->sampleInfo.allSampleSize/(double)object->sampleInfo.mdiaSize;
    
//    printf("=================%%%.01f==================\r",progress*100);
    
    if (leftSize > 0 && leftSize < size)
    {
        size = leftSize;
        MP4DBG("g_Offset=%d dataSize=%d size=%d allsize=%d\n",object->sampleInfo.offset, dataSize,size,object->sampleInfo.allSampleSize);
        MP4Package_FillSampleNode(object,data,size,bIframe, writeCallBack,userParams); 
        ret = -1;
    }  
    if (leftSize > 0 && leftSize > size)
    {
        MP4Package_FillSampleNode(object,data,size,bIframe, writeCallBack,userParams);   
    }
    if (leftSize <= 0)
    { 
    }  
    
    return ret;	
}

int MP4Package_FillSampleTable(MP4_OBJECT_T *object)
{
    int i = 0;
    int chunkId = 1;
    int chunkIdx = 0;
    int sampleCount = 0;
    int bk_sampleNum_preChunck = 0;
    int sampleNum_preChunck = 0;
    
    object->sampleInfo.syncList = (unsigned int *)malloc(object->sampleInfo.IframeNum*4);
    if (object->sampleInfo.syncList == NULL)
    {
        MP4DBG("malloc failed size=%d\n",object->sampleInfo.IframeNum*4);
        return -1;
    }
    memset(object->sampleInfo.syncList,0,object->sampleInfo.IframeNum*4);
    
    object->sampleInfo.chunkList = (unsigned int *)malloc(object->sampleInfo.chunkCount*4*3);
    if (object->sampleInfo.chunkList == NULL)
    {
        MP4DBG("malloc failed size=%d\n",object->sampleInfo.chunkCount*4*3);
        if (object->sampleInfo.syncList)
            free(object->sampleInfo.syncList);
        return -1;
    }
    memset(object->sampleInfo.chunkList,0,object->sampleInfo.chunkCount*4*3);
    
    object->sampleInfo.sizeList = (unsigned int *)malloc(object->sampleInfo.sampleCount*4);
    if (object->sampleInfo.sizeList == NULL)
    {
        MP4DBG("malloc failed size=%d\n",object->sampleInfo.sampleCount);
        if (object->sampleInfo.syncList)
            free(object->sampleInfo.syncList);
        if (object->sampleInfo.chunkList)
            free(object->sampleInfo.chunkList);
        return -1;
    }
    memset(object->sampleInfo.sizeList,0,object->sampleInfo.sampleCount*4);
    
    object->sampleInfo.chunkOffsetList = (unsigned int *)malloc(object->sampleInfo.chunkCount*4);
    if (object->sampleInfo.chunkOffsetList == NULL)
    {
        MP4DBG("malloc failed size=%d\n",object->sampleInfo.sampleCount);
        if (object->sampleInfo.syncList)
            free(object->sampleInfo.syncList);
        if (object->sampleInfo.chunkList)
            free(object->sampleInfo.chunkList);
        if (object->sampleInfo.sizeList)
            free(object->sampleInfo.chunkList);
        return -1;
    }
    memset(object->sampleInfo.chunkOffsetList,0,object->sampleInfo.chunkCount*4);

    int count = 48;	
    MP4_SAMPLE_NODE *pNode = NULL;    
	
    pNode = object->sampleInfo.sampleNodeListTail;
	while(pNode)
//    for (i = 0; i < sampleCount; i++)
    {
        MP4DBG("sampleNum=%d sampleId=%d  bIframe=%d  IframeId =%d   size=%d  chunckId=%d  chunckOffset=%d  offset=%d \n\n",
          sampleNum_preChunck,pNode->sampleId,pNode->bIframe,pNode->IframeId,pNode->size,pNode->chunkId,pNode->chunkOffset,pNode->offset);        
        //stss
        if (pNode && pNode->bIframe)
        {
            object->sampleInfo.syncList[pNode->IframeId-1] = htonl(pNode->sampleId);
        }

        //stsc stco
        if (pNode && pNode->chunkId == chunkId)
        {
            sampleNum_preChunck++;
        }

        count += pNode->size;
        
        if (pNode && pNode->chunkId - chunkId == 1 && pNode->bIframe)
        {
            if (bk_sampleNum_preChunck != sampleNum_preChunck)
            {
                object->sampleInfo.chunkList[chunkIdx*3] = htonl(chunkId);
                object->sampleInfo.chunkList[chunkIdx*3+1] = htonl(sampleNum_preChunck);
                object->sampleInfo.chunkList[chunkIdx*3+2] = htonl(1);
                bk_sampleNum_preChunck = sampleNum_preChunck;
                chunkIdx++;
            }
            object->sampleInfo.chunkOffsetList[chunkId] = htonl(pNode->chunkOffset);
            sampleNum_preChunck = 1;
            chunkId = pNode->chunkId;
        }
        
        if (pNode && pNode->chunkId == object->sampleInfo.chunkCount)
            object->sampleInfo.chunkOffsetList[object->sampleInfo.chunkCount-1] = htonl(pNode->chunkOffset);  

        //stsz
        object->sampleInfo.sizeList[pNode->sampleId-1] = htonl(pNode->size);
		
		pNode = pNode->sampleNodeListPrev;
    }

    if (object->sampleInfo.IframeNum%2 || bk_sampleNum_preChunck != sampleNum_preChunck)
    {
        object->sampleInfo.chunkList[chunkIdx*3] = htonl(object->sampleInfo.chunkCount);
        object->sampleInfo.chunkList[chunkIdx*3+1] = htonl(sampleNum_preChunck);
        object->sampleInfo.chunkList[chunkIdx*3+2] = htonl(1);
        chunkIdx++;
    }
    object->sampleInfo.chunkOffsetList[0] = htonl(48);
      
    MP4_SampleTableBox *sampleTableBox = &object->metadata.trak.mdia.minf.stbl;
    sampleTableBox->stts.fullBoxHeader.size = htonl(sizeof(MP4_DecodTimeBox));
    sampleTableBox->stts.fullBoxHeader.type = htonl(0X73747473);//{'s','t','t','s'}
    sampleTableBox->stts.fullBoxHeader.version = htonl(0X00000000);
    sampleTableBox->stts.entryCount = htonl(0X00000001);
    sampleTableBox->stts.sampleCount = htonl(object->sampleInfo.sampleCount);
    sampleTableBox->stts.sampleDelta = htonl(0X00000001);
   
    sampleTableBox->stss.fullBoxHeader.size = htonl(sizeof(MP4_SyncSampleBox) - 4 + object->sampleInfo.IframeNum * 4);
    sampleTableBox->stss.fullBoxHeader.type = htonl(0X73747373);//{'s','t','s','s'}
    sampleTableBox->stss.fullBoxHeader.version = htonl(0X00000000);
    sampleTableBox->stss.entryCount = htonl(object->sampleInfo.IframeNum);
    sampleTableBox->stss.syncList = object->sampleInfo.syncList;

    sampleTableBox->stsc.fullBoxHeader.size = htonl(sizeof(MP4_SampleToChunkBox) - 4 + chunkIdx * 3 * 4);
    sampleTableBox->stsc.fullBoxHeader.type = htonl(0X73747363);//{'s','t','s','s'}
    sampleTableBox->stsc.fullBoxHeader.version = htonl(0X00000000);
    sampleTableBox->stsc.entryCount = htonl(chunkIdx);
    sampleTableBox->stsc.chunkList = object->sampleInfo.chunkList;

    sampleTableBox->stsz.fullBoxHeader.size = htonl(sizeof(MP4_SampleSizeBox) - 4 + object->sampleInfo.sampleCount * 4);
    sampleTableBox->stsz.fullBoxHeader.type = htonl(0X7374737A);//{'s','t','s','z'}
    sampleTableBox->stsz.fullBoxHeader.version = htonl(0X00000000);
    sampleTableBox->stsz.sampleSize= htonl(0X00000000);
    sampleTableBox->stsz.sampleCount = htonl(object->sampleInfo.sampleCount);
    sampleTableBox->stsz.sizeList = object->sampleInfo.sizeList;    

    sampleTableBox->stco.fullBoxHeader.size = htonl(sizeof(MP4_ChunkOffsetBox) - 4 + object->sampleInfo.chunkCount * 4);
    sampleTableBox->stco.fullBoxHeader.type = htonl(0X7374636F);//{'s','t','c','0'}
    sampleTableBox->stco.fullBoxHeader.version = htonl(0X00000000);
    sampleTableBox->stco.entryCount = htonl(object->sampleInfo.chunkCount);
    sampleTableBox->stco.chunkOffsetList = object->sampleInfo.chunkOffsetList;
    
    sampleTableBox->avcc.boxHeader.size = htonl(sizeof(MP4_AVCDecoderInfoBox) + object->mp4Attr.spsNum - 4 + object->mp4Attr.ppsNum - 4);
    sampleTableBox->avcc.boxHeader.type = htonl(0X61766343);//{'a','v','c','c'}
    sampleTableBox->avcc.configurationVersion = 0x01;
    sampleTableBox->avcc.profileIndication = 0x64;
    sampleTableBox->avcc.profileCompatibility = 0x00;
    sampleTableBox->avcc.levelIndication = 0x28;
    sampleTableBox->avcc.nalSize = 0xFF;
    sampleTableBox->avcc.spsSetsNum = 0xE1;
    sampleTableBox->avcc.spsSize = htons(object->mp4Attr.spsNum);
    sampleTableBox->avcc.sps = object->mp4Attr.spsTable;
    sampleTableBox->avcc.ppsSetsNum = 0x01;
    sampleTableBox->avcc.ppsSize = htons(object->mp4Attr.ppsNum);//htons(g_ppsNum);
    sampleTableBox->avcc.pps = object->mp4Attr.ppsTable;
#if MP4_DEBUG
    int size = (char *)&sampleTableBox->avcc.spsSize - (char *)&sampleTableBox->avcc;
    printf("avcc\n");
    PrintBuffer((char *)&sampleTableBox->avcc, size);
    PrintBuffer(object->mp4Attr.spsTable, object->mp4Attr.spsNum);
    size = (char *)&sampleTableBox->avcc.pps - (char *)&sampleTableBox->avcc.ppsSetsNum;
    PrintBuffer((char *)&sampleTableBox->avcc.ppsSetsNum, size);
    PrintBuffer(object->mp4Attr.ppsTable, object->mp4Attr.ppsNum);
    printf("\n\n\n");
#endif      
    sampleTableBox->avc1.boxHeader.size = htonl(sizeof(MP4_AVC1SampleBox) + htonl(sampleTableBox->avcc.boxHeader.size));
    sampleTableBox->avc1.boxHeader.type = htonl(0X61766331);//{'a','v','c','1'}
    memset(sampleTableBox->avc1.res,0,sizeof(sampleTableBox->avc1.res));
    sampleTableBox->avc1.res[7] = 0x01;
    sampleTableBox->avc1.resolution = htonl(object->mp4Attr.width | (object->mp4Attr.height >> 16));
    //sampleTableBox->.avc1.width = htonl(g_mp4Attr.width >> 16);
    //sampleTableBox->.avc1.height = htonl(g_mp4Attr.height >> 16);
    sampleTableBox->avc1.horizResolution = htonl(0X00480000);
    sampleTableBox->avc1.verResolution = htonl(0X00480000);
    memcpy(sampleTableBox->avc1.compressionName,g_compressionName,sizeof(g_compressionName));
    
    sampleTableBox->stsd.fullBoxHeader.size = htonl(sizeof(MP4_SampleDescriptionBox) + htonl(sampleTableBox->avc1.boxHeader.size));//avc1+avcc
    sampleTableBox->stsd.fullBoxHeader.type = htonl(0X73747364);//{'s','t','s','d'}
    sampleTableBox->stsd.fullBoxHeader.version = htonl(0x00000000);
    sampleTableBox->stsd.res = htonl(0x00000001);

    sampleTableBox->boxHeader.type = htonl(0X7374626C);//{'s','t','b','l'}
    sampleTableBox->boxHeader.size = htonl(sizeof(MP4_BoxHeader)
                                      + htonl(sampleTableBox->stsd.fullBoxHeader.size)
                                      + htonl(sampleTableBox->stts.fullBoxHeader.size)
                                      + htonl(sampleTableBox->stss.fullBoxHeader.size)
                                      + htonl(sampleTableBox->stsc.fullBoxHeader.size)
                                      + htonl(sampleTableBox->stsz.fullBoxHeader.size)
                                      + htonl(sampleTableBox->stco.fullBoxHeader.size));
#if MP4_DEBUG
    MP4DBG("avcc=%d avc1=%d stsd=%d stts=%d stss=%d stsc=%d stsz=%d stco=%d stdl=%d\n",
        htonl(sampleTableBox->avcc.boxHeader.size),
        htonl(sampleTableBox->avc1.boxHeader.size),
        htonl(sampleTableBox->stsd.fullBoxHeader.size),
        htonl(sampleTableBox->stts.fullBoxHeader.size),
        htonl(sampleTableBox->stss.fullBoxHeader.size),
        htonl(sampleTableBox->stsc.fullBoxHeader.size),
        htonl(sampleTableBox->stsz.fullBoxHeader.size),
        htonl(sampleTableBox->stco.fullBoxHeader.size),
        htonl(sampleTableBox->boxHeader.size));
#endif    
    return htonl(sampleTableBox->boxHeader.size);
}


int MP4Package_FillMetadata(MP4_OBJECT_T *object)
{        
    struct tm *te;
    time_t t = time(NULL);
    te = gmtime(&t);
    te->tm_year = te->tm_year;
    t = mktime(te);

    //Sample Table Box [stbl box]
//    object->metadata.trak.mdia.minf.stbl = g_sampleTableBox;

    //Media Information Box [minf box] = [vmhd,dinf,stbl]
    object->metadata.trak.mdia.minf.boxHeader.type = htonl(0X6D696E66);//{'m','i','n','f'}
    object->metadata.trak.mdia.minf.boxHeader.size = htonl(sizeof(MP4_BoxHeader) + sizeof(MP4_VideoHeader) + sizeof(MP4_DataInfoBox) + object->sampleInfo.sampleTableSize) ;    
    //Video Media Header Box [vmhd box]
    object->metadata.trak.mdia.minf.vmhd.fullBoxHeader.size = htonl(sizeof(MP4_VideoHeader));
    object->metadata.trak.mdia.minf.vmhd.fullBoxHeader.type = htonl(0X766D6864);//{'v','m','h','d'}
    object->metadata.trak.mdia.minf.vmhd.fullBoxHeader.version= htonl(0x00000001);
    object->metadata.trak.mdia.minf.vmhd.graphicsMode = htonl(0x00000000);
    memset(object->metadata.trak.mdia.minf.vmhd.opcolor,0,sizeof(object->metadata.trak.mdia.minf.vmhd.opcolor)); 

    //Data Information Box [dinf box] = [dref]
    object->metadata.trak.mdia.minf.dinf.boxHeader.type = htonl(0X64696E66);//{'d','f','i','n'}
    object->metadata.trak.mdia.minf.dinf.boxHeader.size = htonl(sizeof(MP4_DataInfoBox)); 
    //Data Reference Box [dref box] = [url]
    object->metadata.trak.mdia.minf.dinf.dref.fullBoxHeader.size = htonl(sizeof(MP4_DataReferenceBox) + sizeof(MP4_FullBoxHeader)); 
    object->metadata.trak.mdia.minf.dinf.dref.fullBoxHeader.type = htonl(0X64726566);//{'d','r','e','f'}
    object->metadata.trak.mdia.minf.dinf.dref.fullBoxHeader.version= htonl(0x00000000);
    object->metadata.trak.mdia.minf.dinf.dref.entryCount = htonl(0x00000001);
    //Data Entry Url Box [url box]
    object->metadata.trak.mdia.minf.dinf.url.size = htonl(sizeof(MP4_FullBoxHeader));
    object->metadata.trak.mdia.minf.dinf.url.type = htonl(0X75726C20);//{'u','r','i'}
    object->metadata.trak.mdia.minf.dinf.url.version = htonl(0x00000001);//uri�е�flagsΪ1��ʾ��uri�ַ���Ϊ�գ�track�������ı���;

    //Media Box [mdia box] = [mdhd,hdlr,minf]
//    unsigned int *mdiaSize = &object->metadata.trak.mdia.boxHeader.size;
    object->metadata.trak.mdia.boxHeader.type = htonl(0X6D646961);//{'m','d','i','a'}
    object->metadata.trak.mdia.boxHeader.size = htonl(sizeof(MP4_BoxHeader) + sizeof(MP4_MediaHeader) + sizeof(MP4_ReferenceBox) + htonl(object->metadata.trak.mdia.minf.boxHeader.size));  
    //Media Header Box [mdhd]
    object->metadata.trak.mdia.mdhd.fullBoxHeader.size = htonl(sizeof(MP4_MediaHeader));
    object->metadata.trak.mdia.mdhd.fullBoxHeader.type = htonl(0X6D646864);//{'m','d','h','d'}
    object->metadata.trak.mdia.mdhd.fullBoxHeader.version = htonl(0x00000000);
    object->metadata.trak.mdia.mdhd.creationTime = htonl((unsigned int)t);
    object->metadata.trak.mdia.mdhd.modificationTime = htonl((unsigned int)t);
    object->metadata.trak.mdia.mdhd.timeScale = htonl(object->mp4Attr.fps);//��ÿ����֡��
    object->metadata.trak.mdia.mdhd.duration = htonl(object->sampleInfo.sampleCount);//��֡��Ϊ�̶ȵ���֡����
    object->metadata.trak.mdia.mdhd.language = htons(0x55C4);
    object->metadata.trak.mdia.mdhd.preDefined = htons(0x0000);

    //Handler Reference Box [hdlr]
    object->metadata.trak.mdia.hdlr.fullBoxHeader.size = htonl(sizeof(MP4_ReferenceBox));
    object->metadata.trak.mdia.hdlr.fullBoxHeader.type = htonl(0X68646C72);//{'h','d','l','r'}
    object->metadata.trak.mdia.hdlr.fullBoxHeader.version = htonl(0x00000000);
    object->metadata.trak.mdia.hdlr.preDefined = htonl(0x00000000);
    object->metadata.trak.mdia.hdlr.type = htonl(0x76696465);//{'v','i','d','e'}
    memset(object->metadata.trak.mdia.hdlr.res,0,sizeof(object->metadata.trak.mdia.hdlr.res));
    memset(object->metadata.trak.mdia.hdlr.name,0,sizeof(object->metadata.trak.mdia.hdlr.name));
    snprintf(object->metadata.trak.mdia.hdlr.name,sizeof(object->metadata.trak.mdia.hdlr.name),"VideoHandler");  

    //Track Box [track box] = [tkhd,edts,mdia]
//    unsigned int *trakSize = &object->metadata.trak.boxHeader.size;
    object->metadata.trak.boxHeader.size = htonl(sizeof(MP4_BoxHeader) + sizeof(MP4_EditBox) + sizeof(MP4_TrackHeader) + htonl(object->metadata.trak.mdia.boxHeader.size));
    object->metadata.trak.boxHeader.type = htonl(0X7472616B);//{'t','r','a','k'}
    //Track Header Box [tkhd]
    object->metadata.trak.tkhd.fullBoxHeader.size = htonl(sizeof(MP4_TrackHeader));
    object->metadata.trak.tkhd.fullBoxHeader.type = htonl(0X746B6864);//{'t','k','h','d'}
    object->metadata.trak.tkhd.fullBoxHeader.version = htonl(0x0000000F);//[8.24]=[version.flags] flags:0x000001ʹ�ܸ�track 0x000002�ڲ����б����� 0x000004��Ԥ���б�����
    object->metadata.trak.tkhd.creationTime = htonl((unsigned int)t);
    object->metadata.trak.tkhd.modificationTime = htonl((unsigned int)t);
    object->metadata.trak.tkhd.trackId = htonl(0x00000001);
    object->metadata.trak.tkhd.res0 = htonl(0x00000000);
    object->metadata.trak.tkhd.duration = htonl(object->mp4Attr.druation);
    memset(object->metadata.trak.tkhd.res1,0,sizeof(object->metadata.trak.tkhd.res1));
    object->metadata.trak.tkhd.layer = htons(0x0000);
    object->metadata.trak.tkhd.alternateGroup = htons(0x0000);
    object->metadata.trak.tkhd.volume = htons(0x0100);
    object->metadata.trak.tkhd.res2 = htons(0x0000);
    memcpy(object->metadata.trak.tkhd.matrix,g_matrix,sizeof(g_matrix));
    object->metadata.trak.tkhd.width = htonl(object->mp4Attr.width);
    object->metadata.trak.tkhd.height = htonl(object->mp4Attr.height);
    
    //Edit Box [edts box] = [elst]
    object->metadata.trak.edts.boxHeader.size = htonl(sizeof(MP4_EditBox));
    object->metadata.trak.edts.boxHeader.type = htonl(0X65647473);//{'e','d','t','s'}
    //Edit List Box [dlst box]
    object->metadata.trak.edts.elst.fullBoxHeader.size = htonl(sizeof(MP4_EditListBox));
    object->metadata.trak.edts.elst.fullBoxHeader.type = htonl(0X656C7374);//{'e','l','s','t'}
    object->metadata.trak.edts.elst.fullBoxHeader.version = htonl(0x00000000);
    object->metadata.trak.edts.elst.entryCount = htonl(0x00000001);
    object->metadata.trak.edts.elst.segmentDuration = htonl(object->mp4Attr.druation);
    object->metadata.trak.edts.elst.mediaRate = htonl(0x00010000);//[16.16]    

    //Movie Box [moov box] = [mvhd,trak,udta]
//    unsigned int *moovSize = &object->metadata.moov.size;
    object->metadata.moov.size = htonl(sizeof(MP4_BoxHeader) + sizeof(MP4_MovieHeader) + sizeof(MP4_UserBox) + htonl(object->metadata.trak.boxHeader.size));
    object->metadata.moov.type = htonl(0X6D6F6F76);//{'m','o','o','v'}
    //Movie Header Box [mvhd]
    object->metadata.mvhd.fullBoxHeader.size = htonl(sizeof(MP4_MovieHeader));
    object->metadata.mvhd.fullBoxHeader.type = htonl(0X6D766864);//{'m','v','h','d'}
    object->metadata.mvhd.fullBoxHeader.version = htonl(0x00000000);
    object->metadata.mvhd.creationTime = htonl((unsigned int)t);
    object->metadata.mvhd.modificationTime = htonl((unsigned int)t);
    object->metadata.mvhd.timeScale = htonl(object->mp4Attr.timeScan);
    object->metadata.mvhd.duration = htonl(object->mp4Attr.druation);
    object->metadata.mvhd.rate = htonl(object->mp4Attr.rate);
    object->metadata.mvhd.volume = htons(object->mp4Attr.volume);
    memset(object->metadata.mvhd.res,0,sizeof(object->metadata.mvhd.res));
    memcpy(object->metadata.mvhd.matrix,g_matrix,sizeof(g_matrix));
    memcpy(object->metadata.mvhd.pre_defined,g_pre_defined,sizeof(g_pre_defined));
    object->metadata.mvhd.nextTrackId = htonl(0x00000002);           

    //User Data box  [udta box] = [meta]
    object->metadata.udta.boxHeader.size = htonl(sizeof(MP4_UserBox));
    object->metadata.udta.boxHeader.type = htonl(0X75647461);//{'u','d','t','a'}
    //[meta box] = [hdlr,ilst]
    object->metadata.udta.meta.size = htonl(sizeof(MP4_FullBoxHeader) + sizeof(MP4_ReferenceBox) + sizeof(MP4_UserDataBox));
    object->metadata.udta.meta.type = htonl(0X6D657461);//{'m','e','t','a'}
    object->metadata.udta.meta.version = htonl(0X00000000);
    // Handler Reference Box [hdlr box]
    object->metadata.udta.hdlr.fullBoxHeader.size = htonl(sizeof(MP4_ReferenceBox));
    object->metadata.udta.hdlr.fullBoxHeader.type = htonl(0X68646C72);//{'h','d','l','r'}
    object->metadata.udta.hdlr.fullBoxHeader.version = htonl(0x00000000);
    object->metadata.udta.hdlr.preDefined = htonl(0x00000000);
    object->metadata.udta.hdlr.type = htonl(0x6D646972) ;//{'m','d','i','r'}
    memset(object->metadata.udta.hdlr.res,0,sizeof(object->metadata.udta.hdlr.res));
    memset(object->metadata.udta.hdlr.name,0,sizeof(object->metadata.udta.hdlr.name));
    snprintf(object->metadata.udta.hdlr.name,sizeof(object->metadata.udta.hdlr.name),"%s","application");
    //data box [ilst box] 
    object->metadata.udta.ilst.boxHeader.size = htonl(sizeof(MP4_UserDataBox));
    object->metadata.udta.ilst.boxHeader.type = htonl(0X696C7374);//{'i','l','s','t'}
    object->metadata.udta.ilst.tagLength = htonl(sizeof(object->metadata.udta.ilst.payload)+6*4);//��ͷ����Ĵ�С
    object->metadata.udta.ilst.tagName = htonl(0x4F564653);//{'o','v','f','s'}
    object->metadata.udta.ilst.dataLength = htonl(sizeof(object->metadata.udta.ilst.payload)+4*4); 
    object->metadata.udta.ilst.dataTag = htonl(0x6C757877);//{'l','u','x','w'}
    object->metadata.udta.ilst.dataType = htonl(0x00000000);
    memset(object->metadata.udta.ilst.payload,0,sizeof(object->metadata.udta.ilst.payload));
    snprintf(object->metadata.udta.ilst.payload,sizeof(object->metadata.udta.ilst.payload),"%s0","hello Mp4");    
/*
	//���û�������������������ݱ�֤�ļ���Сһ��
	g_fileSize = 40 + g_mdiaSize + htonl(object->metadata.moov.size);
	unsigned int extSize = g_mp4Attr.fileSize - g_fileSize;
	LOGW("g_mdiaSize=%d moovSize=%d g_fileSize=%d extSize=%d\n",g_mdiaSize,htonl(object->metadata.moov.size),g_fileSize,extSize);
	object->metadata.udta.ilst.dataLength = htonl(htonl(object->metadata.udta.ilst.dataLength) + extSize);
	object->metadata.udta.ilst.tagLength = htonl(htonl(object->metadata.udta.ilst.tagLength) + extSize);
	object->metadata.udta.ilst.boxHeader.size = htonl(htonl(object->metadata.udta.ilst.boxHeader.size) + extSize);
	object->metadata.udta.boxHeader.size = htonl(htonl(object->metadata.udta.boxHeader.size) + extSize);
	object->metadata.moov.size = htonl(htonl(object->metadata.moov.size) + extSize);
*/
#if MP4_DEBUG
//    printf("\n\n",htonl(g_sampleTableBox.boxHeader.size));
//    printf("stblSize=%d ",htonl(g_sampleTableBox.boxHeader.size));
    printf("minfSize=%d ",htonl(object->metadata.trak.mdia.minf.boxHeader.size));
    printf("dinfSize=%d ",htonl(object->metadata.trak.mdia.minf.dinf.boxHeader.size));
    printf("mdiaSize=%d ",htonl(object->metadata.trak.mdia.boxHeader.size));
    printf("mdiaSize=%d ",htonl(object->metadata.trak.boxHeader.size));   
    printf("moovSize=%d ",htonl(object->metadata.moov.size)); 
//    printf("\n\n",htonl(g_sampleTableBox.boxHeader.size));
#endif    
    return 0;
}


int MP4Package_WriteMetadata(uint32 handle,MP4PACKAGE_WRITESAMPLE writeCallBack,void *userParams)
{
    if (writeCallBack == NULL)
    {
        MP4DBG("param error!\n");
        return -1;
    }

	uint32 idx = 0;
	MP4_OBJECT_T *object = NULL;
	idx = CheckObject(handle);
	if (idx < 0)
	{
		MP4DBG("check handle failed!\n");
		return -1;
	}
	object = g_mp4Manager.objects[idx];

    MP4DBG("write sample end\n");
    
//    g_mp4Attr.druation = (g_stopTime - g_startTime)*1000;
//    g_mp4Attr.fps = g_sampleCount/(g_stopTime - g_startTime);

	MP4DBG("===3=======write sample table info==========\n");
	
    int ret = 0;
	object->sampleInfo.sampleTableSize = MP4Package_FillSampleTable(object);
//    g_sampleTableSize = MP4Package_FillSampleTable();
    if (object->sampleInfo.sampleTableSize < 0)
    {
        MP4DBG("get sample table failed!\n");
        return -1;
    }
	
 MP4DBG("==4========write Metadata info\n=========");   
    ret = MP4Package_FillMetadata(object);   
    if (ret != 0)
    {
        MP4DBG("metadata error\n");
        return -1;
    }   

 MP4DBG("====5========send http data\n=============");      
 
    int size = 0;
    size = (char *)&object->metadata.trak.mdia.minf.dinf.url - (char *)&object->metadata + sizeof(MP4_FullBoxHeader);
    writeCallBack((char *)&object->metadata,size,userParams);
#if MP4_DEBUG    
    printf("=================moov======================\n");
    PrintBuffer((char *)&object->metadata, size);
    printf("\n\n\n");
#endif
    size = (char *)&object->metadata.trak.mdia.minf.stbl.avc1 - (char *)&object->metadata.trak.mdia.minf.stbl + sizeof(MP4_AVC1SampleBox);
    writeCallBack((char *)&object->metadata.trak.mdia.minf.stbl,size,userParams);
#if MP4_DEBUG    
    printf("=================stbl======================\n");
    PrintBuffer((char *)&object->metadata.trak.mdia.minf.stbl, size);
    printf("\n\n\n");
#endif    

    char *avccBuf = (char *)malloc(htonl(object->metadata.trak.mdia.minf.stbl.avcc.boxHeader.size));
    if (avccBuf == NULL)
    {
        MP4DBG("malloc failed size=%d!",htonl(object->metadata.trak.mdia.minf.stbl.avcc.boxHeader.size));
        return -1;
    }
    size = (char *)&object->metadata.trak.mdia.minf.stbl.avcc.sps - (char *)&object->metadata.trak.mdia.minf.stbl.avcc;
    memcpy(avccBuf,(char *)&object->metadata.trak.mdia.minf.stbl.avcc,size);
    memcpy(avccBuf+size,(char *)object->mp4Attr.spsTable,object->mp4Attr.spsNum);
    memcpy(avccBuf+size+object->mp4Attr.spsNum,(char *)&object->metadata.trak.mdia.minf.stbl.avcc.ppsSetsNum,3);
    memcpy(avccBuf+size+object->mp4Attr.spsNum+3,object->mp4Attr.ppsTable,object->mp4Attr.ppsNum);
    size = htonl(object->metadata.trak.mdia.minf.stbl.avcc.boxHeader.size);
    writeCallBack((char *)avccBuf,size,userParams);
    free(avccBuf);
#if MP4_DEBUG    
    printf("g_spsNum=%d  ppsSize=%d\n",object->mp4Attr.spsNum,object->mp4Attr.ppsNum);
    printf("=================avccBuf======================\n");
    PrintBuffer((char *)avccBuf, size);
    printf("\n\n\n");
#endif    

    size = htonl(object->metadata.trak.mdia.minf.stbl.stts.fullBoxHeader.size);
    writeCallBack((char *)&object->metadata.trak.mdia.minf.stbl.stts,size,userParams);
#if MP4_DEBUG    
    printf("=================stts========sampleCount=%d==============\n",object->sampleInfo.sampleCount);
    PrintBuffer((char *)&object->metadata.trak.mdia.minf.stbl.stts, size);
    printf("\n\n\n");
#endif    
    size = sizeof(MP4_FullBoxHeader) + 4;
    writeCallBack((char *)&object->metadata.trak.mdia.minf.stbl.stss,size,userParams);  
#if MP4_DEBUG    
    printf("=================stss==========IframeNum=%d===========\n",object->sampleInfo.IframeNum);
    PrintBuffer((char *)&object->metadata.trak.mdia.minf.stbl.stss, size); 
#endif      
    size = object->sampleInfo.IframeNum*4;
    writeCallBack((char *)object->sampleInfo.syncList,size,userParams); 
#if MP4_DEBUG      
    PrintBuffer((char *)object->sampleInfo.syncList, size);  
    printf("\n\n\n"); 
#endif    
    
    size = sizeof(MP4_FullBoxHeader) + 4;
    writeCallBack((char *)&object->metadata.trak.mdia.minf.stbl.stsc,size,userParams);  
#if MP4_DEBUG    
    printf("=================stsc======================\n");
    PrintBuffer((char *)&object->metadata.trak.mdia.minf.stbl.stsc, size); 
#endif    
    size = (htonl(object->metadata.trak.mdia.minf.stbl.stsc.fullBoxHeader.size) - sizeof(MP4_SampleToChunkBox) + 4);
    writeCallBack((char *)object->sampleInfo.chunkList,size,userParams);
#if MP4_DEBUG     
    PrintBuffer((char *)object->sampleInfo.chunkList, size);    
    printf("\n\n\n");   
#endif    
    size = sizeof(MP4_FullBoxHeader) + 8;
    writeCallBack((char *)&object->metadata.trak.mdia.minf.stbl.stsz,size,userParams); 
#if MP4_DEBUG      
    printf("=================stsz==========sampleCount=%d===========\n",object->sampleInfo.sampleCount);
    PrintBuffer((char *)&object->metadata.trak.mdia.minf.stbl.stsz, size);   
#endif    
    size = object->sampleInfo.sampleCount*4;
    writeCallBack((char *)object->sampleInfo.sizeList,size,userParams); 
#if MP4_DEBUG      
    PrintBuffer((char *)object->sampleInfo.sizeList, size);    
    printf("\n\n\n");
#endif     
    size = sizeof(MP4_FullBoxHeader) + 4;
    writeCallBack((char *)&object->metadata.trak.mdia.minf.stbl.stco,size,userParams); 
#if MP4_DEBUG      
    printf("=================stco=========chunkCount=%d============\n",object->sampleInfo.chunkCount);
    PrintBuffer((char *)&object->metadata.trak.mdia.minf.stbl.stco, size);  
#endif    
    size = object->sampleInfo.chunkCount*4;
    writeCallBack((char *)object->sampleInfo.chunkOffsetList,size,userParams); 

#if MP4_DEBUG      
    PrintBuffer((char *)object->sampleInfo.chunkOffsetList, size);    
    printf("\n\n\n");     
    MP4DBG("g_Offset=%d  g_IframeNum=%d  g_chunkCount=%d  g_sampleCount=%d\n",object->sampleInfo.offset,object->sampleInfo.IframeNum,object->sampleInfo.chunkCount,object->sampleInfo.sampleCount);      
    printf("\n\n\n");
#endif     
    size = htonl(object->metadata.udta.boxHeader.size);
	//size = sizeof(MP4_UserDataBox);
    writeCallBack((char *)&object->metadata.udta,size,userParams);
#if MP4_DEBUG      
    printf("=================udta======================\n");
    PrintBuffer((char *)&object->metadata.udta, size);
#endif     
    printf("\n\n\n");

	MP4_BoxHeader fre = {0};
	char *extData = NULL;
	uint32 fileSize = 40 + object->sampleInfo.mdiaSize + htonl(object->metadata.moov.size);
	unsigned int extSize = object->mp4Attr.fileSize - fileSize;
    fre.size = htonl(extSize);
    fre.type = htonl(0X66726565);//{'f','r','e','e'}	
    writeCallBack((char *)&fre,8,userParams);
	extData = (char *)malloc(extSize-8);
	if (extData == NULL)
	{
		MP4DBG("malloc failed! [size=%d]\n",extSize-8);
	}
	else
	{
		memset(extData,0,extSize-8);
		writeCallBack(extData,extSize-8,userParams);
		free(extData);
	}	
	MP4DBG("g_fileHeard=%d g_mdiaSize=%d moovSize=%d  extSize=%d g_fileSize=%d\n",40,object->sampleInfo.mdiaSize,htonl(object->metadata.moov.size),extSize,fileSize+extSize);
/*
	char *extData = NULL;
	size = g_mp4Attr.fileSize - g_fileSize;
	LOGW("extSize=%d\n",size);
	if (size > 0)
	{
		extData = (char *)Common_Malloc(size,4,__FUNCTION__,__LINE__);
		memset(extData,0,size);
		if (extData == NULL)
		{
			LOGE("malloc failed! [size=%d]\n",extData);
		}
		else
		{
			writeCallBack(extData,size,userParams);
			Common_Free(extData,__FUNCTION__,__LINE__);
		}
	}
*/	
    return 0;	
}

/*
void main()
{
	uint32 i = 0;
	uint32 idx = 0;
	uint32 handle[128] = {0};

//create
	for (i = 0; i < 10; i++)
	{
		handle[i] = MP4Package_CreateMP4Object();
		if (handle[i] < 0)
		{
			MP4DBG("create object failed!\n");
			continue;
		}
		idx = ((handle[i] >> 8) - 1) & 0xff;
//		MP4DBG("i=%d handle=%d index=%d object=%p objectId=%d count=%d\n",i, handle[i], idx, g_mp4Manager.objects[idx],g_mp4Manager.objects[idx]->objectId,handle[i] & 0xff);
	}
	MP4DBG("count=%d head=%p tail=%p\n",g_mp4Manager.objectCount,g_mp4Manager.objectsListHead,g_mp4Manager.objectsListTail);


//H264ReadEncodeInfo
	char data[] = {0x00,0x00,0x00,0x01,0x67,0x23,0x46,0x64,0x63,0x98,0x34,0x63,0x34,0x66,
				   0x00,0x00,0x00,0x01,0x68,0x32,0x64,0x46,0x36,0x89,0x00,0x00,0x00,0x01,
				   0x65,0x23,0x46,0x64,0x63,0x98,0x34,0x63,0x34,0x66,0x43,0x36,0x43,0x66,
				   0x67,0x23,0x46,0x64,0x63,0x98,0x34,0x63,0x34,0x66,0x43,0x36,0x43,0x66};
	char *sps = NULL;
	uint32 spsNum = 0;
	char *pps = NULL;
	uint32 ppsNum = 0;
	MP4Package_H264ReadEncodeInfo(handle[0], data, sizeof(data),&sps, &spsNum, &pps, &ppsNum);
	MP4DBG("spsnum=%d   ppsnum=%d\n",spsNum,ppsNum);

	
//set attr
	MP4_ATTR_T attr = {0};
	memset(&attr,0,sizeof(MP4_ATTR_T));
	attr.fps = 25;			   //ѡ��
	attr.druation = 60*1000;   //60000ms 1min ѡ��
	attr.timeScan = 1000;	   //1000ms ������
	attr.width = 1028 << 16;   //������
	attr.height = 1290 << 16;  //������
	attr.volume = 1 << 8;	   //������
	attr.rate = 1 << 16;	   //������
	attr.startTime = 124134;
	attr.stopTime = 436234;

	attr.spsTable = sps;
	attr.spsNum = spsNum;
	attr.ppsTable = pps;
	attr.ppsNum = ppsNum;
	attr.type = 0;//0-ʵʱ����		 1-���ļ�
//	��ʹ��ʵʱ������ʱ��,������д
	attr.sampleSize = 3*1024*1024;//������
	attr.fileSize = 3*1024*1024 + 1024*10;
	
	MP4Package_SetMP4Attr(handle[0],&attr);
	MP4DBG("fps=%d druation=%d timeScan=%d width=%d height=%d volume=%d rate=%d startTime=%d stopTime=%d\n",
		g_mp4Manager.objects[0]->mp4Attr.fps,g_mp4Manager.objects[0]->mp4Attr.druation,g_mp4Manager.objects[0]->mp4Attr.timeScan,
		g_mp4Manager.objects[0]->mp4Attr.width,g_mp4Manager.objects[0]->mp4Attr.height,g_mp4Manager.objects[0]->mp4Attr.volume,
		g_mp4Manager.objects[0]->mp4Attr.rate,g_mp4Manager.objects[0]->mp4Attr.startTime,g_mp4Manager.objects[0]->mp4Attr.stopTime);

//write head
	FILE *fp = fopen("./mpeg4.mp4","w+");
	if (fp)
	{
		MP4Package_WriteHead(handle[0], data, sizeof(data), Mp4WriteData, (void *)fp);
	}

//write video sample
	MP4Package_WriteSample(handle[0],data,sizeof(data),Mp4WriteData,(void *)fp);
	MP4Package_WriteSample(handle[0],data,0,Mp4WriteData,(void *)fp);
	MP4Package_WriteMetadata(handle[0],Mp4WriteData,(void *)fp);
	fclose(fp);

//destory
	for (i = 0; i < 10; i++)
	{
		idx = ((handle[i] >> 8) - 1) & 0xff;
//		MP4DBG("i=%d handle=%d index=%d object=%p objectId=%d count=%d\n",i, handle[i], idx, g_mp4Manager.objects[idx],g_mp4Manager.objects[idx]->objectId,handle[i] & 0xff);
		MP4Package_DestoryMP4Object(handle[i]);
	}
	MP4DBG("count=%d head=%p tail=%p\n",g_mp4Manager.objectCount,g_mp4Manager.objectsListHead,g_mp4Manager.objectsListTail);

	return ;
}
*/




























