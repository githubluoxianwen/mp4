#ifndef __OVFS_IAPI_MP4_H__
#define __OVFS_IAPI_MP4_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#pragma  pack(push,1)

/*****************************ע��***********************************************
*һ�ֽڶ���Ľṹ�壬�ڸ������Ա�ĸ�ֵ��ʱ�򣬻��Դ�˴洢
*********************************************************************************/
#define MP4_MAX_OBJECT 128
#define int32  int
#define uint32 unsigned int
#define uint16 unsigned short

typedef void (*MP4PACKAGE_WRITESAMPLE)(void *data,uint32 dataSize,void *userParams);


typedef struct
{
    uint32 size;
    uint32 type;
}__attribute__((aligned(1))) MP4_BoxHeader;

typedef struct
{
    uint32 size;
    uint32 type;
    uint32 version;//[8.12]=[version.flag]
}__attribute__((aligned(1))) MP4_FullBoxHeader;

typedef struct
{
    MP4_BoxHeader boxHeader;
    uint32 majorBrand;
    uint32 minorVersion;
    uint32 compatibleBrands[4];
}__attribute__((aligned(1))) MP4_FtypInfo;

typedef struct
{
	MP4_FullBoxHeader fullBoxHeader;
    uint32 creationTime;  //1904.01.01�����ڵ�����
    uint32 modificationTime;
    uint32 timeScale;
    uint32 duration;
    uint32 rate;      //[16.16]=[��.��]
    uint16 volume;  //[8.8]
    char res[10];           //����
    char matrix[36];        //��Ƶ�任����
    char pre_defined[24];
    uint32 nextTrackId;
    
}__attribute__((aligned(1))) MP4_MovieHeader;

typedef struct
{
	MP4_FullBoxHeader fullBoxHeader;// fullBoxHeader.version [8.24]=[version.flags] flags:0x000001ʹ�ܸ�track 0x000002�ڲ����б����� 0x000004��Ԥ���б�����
    uint32 creationTime;
    uint32 modificationTime;
    uint32 trackId;
    uint32 res0;
    uint32 duration;
    char res1[8];//����λ
    uint16 layer;//��Ƶ�㣬Ĭ��Ϊ0,ֵС�����ϲ�
    uint16 alternateGroup;//track������Ϣ��Ĭ��Ϊ0����ʾ������tackû����Ⱥ��ϵ
    uint16 volume;//[8.8]��ʽ�����Ϊ��Ƶtrack��1.0(0x0100)��ʾ�������
    uint16 res2;
    char matrix[36];//��Ƶ�任����
    uint32 width;//
    uint32 height;
}__attribute__((aligned(1))) MP4_TrackHeader;


typedef struct
{
	MP4_FullBoxHeader fullBoxHeader;
    uint32 entryCount;
    uint32 segmentDuration;
    uint32 mediaTime;
    uint32 mediaRate; //[16.16] [����.С��]
}__attribute__((aligned(1))) MP4_EditListBox;

typedef struct
{
	MP4_FullBoxHeader fullBoxHeader;
    uint32 creationTime;
    uint32 modificationTime;
    uint32 timeScale;//��ÿ����֡��
    uint32 duration;//��֡��Ϊ�̶ȵ���֡����
    uint16 language;//ý�������롣���λΪ0������15λΪ3���ַ�
    uint16 preDefined;
}__attribute__((aligned(1))) MP4_MediaHeader;

typedef struct
{
	MP4_FullBoxHeader fullBoxHeader;
    uint32 preDefined;
    uint32 type; //vide soun(audio) hint
    char res[12];
    char name[20];
}__attribute__((aligned(1))) MP4_ReferenceBox;

typedef struct
{
	MP4_FullBoxHeader fullBoxHeader;
    uint32 graphicsMode;//��Ƶ�ϳ�ģʽ,Ϊ0ʱ����ԭʼͼ�񣬷�����opcolor���кϳ�
    char opcolor[4]; //[2.2.2]=[red,green,blue]
}__attribute__((aligned(1))) MP4_VideoHeader;

typedef struct
{
	MP4_FullBoxHeader fullBoxHeader;
    uint32 entryCount;
}__attribute__((aligned(1))) MP4_DataReferenceBox;

typedef struct
{
	MP4_FullBoxHeader fullBoxHeader;
    uint32 res;
}__attribute__((aligned(1))) MP4_SampleDescriptionBox;

typedef struct
{
    MP4_BoxHeader boxHeader;
    char configurationVersion;
    char profileIndication;
    char profileCompatibility;
    char levelIndication;
    char nalSize;//[6.2]=[res,nalsize]
    char spsSetsNum;//[3.5]=[res,num]
    uint16 spsSize;
    char *sps;
    char ppsSetsNum;
    uint16 ppsSize;
    char *pps;
}__attribute__((aligned(1))) MP4_AVCDecoderInfoBox;

typedef struct
{
	MP4_BoxHeader boxHeader;
	char res[24];
    uint32 resolution;//[16.16] [width,height]
    uint32 horizResolution;
    uint32 verResolution;
    char compressionName[42];
}__attribute__((aligned(1))) MP4_AVC1SampleBox;

typedef struct
{
    MP4_FullBoxHeader fullBoxHeader;
    uint32 entryCount; //Ĭ��Ϊ1
    uint32 sampleCount;//I֡
    uint32 sampleDelta; //Ĭ��Ϊ1
}__attribute__((aligned(1))) MP4_DecodTimeBox;

typedef struct
{
    MP4_FullBoxHeader fullBoxHeader;
    uint32 entryCount;
    uint32 *syncList;
}__attribute__((aligned(1)))MP4_SyncSampleBox;

typedef struct
{
    MP4_FullBoxHeader fullBoxHeader;
    uint32 sampleSize;
    uint32 sampleCount;
    uint32 *sizeList;
}__attribute__((aligned(1)))MP4_SampleSizeBox;

typedef struct
{    
    MP4_FullBoxHeader fullBoxHeader;
    uint32 entryCount;
    uint32 *chunkList;
}__attribute__((aligned(1)))MP4_SampleToChunkBox;

typedef struct
{
    MP4_FullBoxHeader fullBoxHeader;
    uint32 entryCount;
    uint32 *chunkOffsetList;
}__attribute__((aligned(1))) MP4_ChunkOffsetBox;

typedef struct
{
    MP4_BoxHeader boxHeader;
    MP4_SampleDescriptionBox stsd;
    MP4_AVC1SampleBox avc1;
    MP4_AVCDecoderInfoBox avcc;
    MP4_DecodTimeBox stts;
    MP4_SyncSampleBox stss;
    MP4_SampleToChunkBox stsc;
    MP4_SampleSizeBox stsz;
    MP4_ChunkOffsetBox stco;
}__attribute__((aligned(1))) MP4_SampleTableBox;

typedef struct
{
	MP4_BoxHeader boxHeader;
    uint32 tagLength;
    uint32 tagName;
    uint32 dataLength;
    uint32 dataTag;
    uint32 dataType;
    uint32 reserved;
    char payload[64];
}__attribute__((aligned(1))) MP4_UserDataBox;

typedef struct
{
    MP4_FullBoxHeader fullBoxHeader;
    //char *uriString;
}__attribute__((aligned(1))) MP4_UrlBox;

typedef struct
{
    MP4_BoxHeader boxHeader;
    MP4_DataReferenceBox dref;
    MP4_FullBoxHeader url;
}__attribute__((aligned(1))) MP4_DataInfoBox;

typedef struct
{
    MP4_BoxHeader boxHeader;
    MP4_VideoHeader vmhd;
    MP4_DataInfoBox dinf;
    MP4_SampleTableBox stbl;
}__attribute__((aligned(1))) MP4_MediaInfoBox;

typedef struct
{
    MP4_BoxHeader boxHeader;
    MP4_MediaHeader mdhd;
    MP4_ReferenceBox hdlr;
    MP4_MediaInfoBox minf;
}__attribute__((aligned(1))) MP4_MediaBox;

typedef struct
{
    MP4_BoxHeader boxHeader;
    MP4_EditListBox elst;
}__attribute__((aligned(1))) MP4_EditBox;

typedef struct
{
    MP4_BoxHeader boxHeader;
    MP4_TrackHeader tkhd;
    MP4_EditBox edts;
    MP4_MediaBox mdia;
}__attribute__((aligned(1))) MP4_TrakBox;

typedef struct
{
    MP4_BoxHeader boxHeader;
    MP4_FullBoxHeader meta;
    MP4_ReferenceBox hdlr;
    MP4_UserDataBox ilst;
}__attribute__((aligned(1)))MP4_UserBox;

typedef struct
{
    MP4_BoxHeader moov;
    MP4_MovieHeader mvhd;
    MP4_TrakBox trak;
    MP4_UserBox udta;
}__attribute__((aligned(1))) MP4_MovieBox;


/*
moov
  |--mvhd
  |
  |--trak
  |   |--tkhd
  |   |
  |   |--edts
  |   |   |--elst
  |   |
  |   |--mdia
  |       |--mdhd
  |       |
  |       |--hdlr
  |       |
  |       |--minf
  |           |--vmhd
  |           |
  |           |--dinf
  |           |    |--dref
  |           |       |--url
  |           |       
  |           |--stbl
  |              |--stsd
  |              |    |--avc1
  |              |        |--avcc
  |              |
  |              |--stts
  |              |
  |              |--stss
  |              |
  |              |--stsc
  |              |
  |              |--stsz
  |              |
  |              |--stco
  |
  |--udta
      |--meta
         |--hdlr
         |
         |--ilst

*/

#if 0
#endif

typedef struct _tagMP4_SAMPLE_NODE
{
    uint32 trackId;     //������Ƶ����               Ĭ��Ϊ1
    uint32 chunkId;    //������Ƶ����                ÿ֡����Ӧ��ǰchunkId
    //uint32 chunckNum;   //������Ƶ��ӵ�е�sample��
    uint32 chunkOffset;//������Ƶ���ȫ��ƫ��          ��Ƶ��ĵ�һ֡Ϊ��ƫ�ƣ�����֡Ϊ0
    uint32 delta;
    uint32 sampleId;    //֡��
    uint32 IframeId;    //I֡��                        ΪI֡ʱ����ֵ,Ĭ��Ϊ0
    uint32 bIframe;     //sample�Ƿ�Ϊ�ؼ�֡
    uint32 offset;      //sample��ȫ�ֵ�ƫ��
    uint32 size;        //sample��ȫ�ֵĴ�С
	
	struct _tagMP4_SAMPLE_NODE *sampleNodeListPrev;
	struct _tagMP4_SAMPLE_NODE *sampleNodeListNext;
}MP4_SAMPLE_NODE;


typedef struct
{
    MP4_FtypInfo ftyp;    //MP4�ļ�ͷ
    MP4_BoxHeader free;   //��������
    MP4_BoxHeader mdat;   //��Ƶ���ݿ�
}__attribute__((aligned(1)))MP4_FILE_HEADER;

typedef struct
{
	uint32 offset;
	uint32 mdiaSize;
	uint32 chunkCount;
	uint32 chunkOffset;
	uint32 sampleCount;
	uint32 IframeNum;
	uint32 allSampleSize;
	
	uint32 sampleTableSize;
	uint32 *syncList;
	uint32 *chunkList;
	uint32 *sizeList;
	uint32 *chunkOffsetList;
	
	MP4_SAMPLE_NODE *sampleNodeListHead;
	MP4_SAMPLE_NODE *sampleNodeListTail;
}MP4_SAMPLE_INFO_T;

typedef struct
{
	uint32 type;
	uint32 fileSize;
	uint32 sampleSize;
	uint32 spsNum;
	char * spsTable;
	uint32 ppsNum;
	char *ppsTable;
	
	uint32 timeScan; //ʱ��̶�(��λms)
	uint32 druation; //��Ƶʱ��(��λms)
	uint32 width;	 //�ֱ��ʿ�
	uint32 height;	 //�ֱ��ʸ�
	uint16 volume;	 //[8.8]��ʽ 0x0100�������
	uint32 rate;	 //[16.16]��ʽ 0x00010000������������
	uint32 fps;
	uint32 startTime;
	uint32 stopTime;
}__attribute__((aligned(1))) MP4_ATTR_T;

typedef struct _tagMP4_OBJECT_T
{
	uint32 objectId;
	MP4_FILE_HEADER fileHeader;
	MP4_ATTR_T mp4Attr;
	MP4_MovieBox metadata;
	
	MP4_SAMPLE_INFO_T sampleInfo;
	MP4PACKAGE_WRITESAMPLE writeCallBack;
	
	struct _tagMP4_OBJECT_T *objectPrev;
	struct _tagMP4_OBJECT_T *objectNext;
}MP4_OBJECT_T;

typedef struct
{
	uint32 objectCount;
	pthread_mutex_t objectMutex;
	MP4_OBJECT_T *objects[MP4_MAX_OBJECT];
	MP4_OBJECT_T *objectsListHead;
	MP4_OBJECT_T *objectsListTail;
}MP4_MANAGER;


int MP4Package_CreateMP4Object();
int MP4Package_H264ReadEncodeInfo(uint32 mp4Handle, char *data, uint32 dataSize,char **sps,uint32 *spsNum,char **pps, uint32 *ppsNum);
int MP4Package_SetMP4Attr(uint32 handle, MP4_ATTR_T *attr);
int MP4Package_DestoryMP4Object(uint handle);
int MP4Package_WriteSample(uint32 handle,void *data,uint32 dataSize,MP4PACKAGE_WRITESAMPLE writeCallBack,void *userParams);
int MP4Package_WriteHead(uint32 handle,void *data,uint32 dataSize,MP4PACKAGE_WRITESAMPLE writeCallBack,void *userParams);
int MP4Package_WriteMetadata(uint32 handle,MP4PACKAGE_WRITESAMPLE writeCallBack,void *userParams);

#pragma  pack(pop)

#endif //__OVFS_IAPI_MP4_H__


