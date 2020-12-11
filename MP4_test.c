#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define MP4_DEBUG_FILE "mpeg4.mp4"

#define MIN(a,b) (a<b?a:b)
void main()
{
	unsigned int offset = 0;
	unsigned int buf[100] = {0};
	int fd = 0;
	int ret = 0;
	fd = open(MP4_DEBUG_FILE,O_RDONLY);
	
	//read ftyp
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X66747970)
		{
			printf("tfypSize=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
			printf("not find ftyp [type=%d]\n",htonl(buf[1]));
	}

	//read free
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X66726565)
		{
			printf("freeSize=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
			printf("not find free [type=%d]\n",htonl(buf[1]));
	}	
	
	//read mdat
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X6D646174)
		{
			printf("mdatSize=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
			printf("not find mdat [type=%d]\n",htonl(buf[1]));
	}
	
	//read moov
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X6D6F6F76)
		{
			printf("moovSize=%d StartOffset=%d\n",htonl(buf[0]),offset);
			//offset += htonl(buf[0]);
		}
		else
		{
			printf("not find moov [type=%d]\n",htonl(buf[1]));
		}
	}	
	
	//read mvhd 
	offset += 8;
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X6D766864)
		{
			printf("mvhdSize=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
			printf("not find mvhd [type=%d]\n",htonl(buf[1]));
	}	
	
	//read trak 
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X7472616B)
		{
			printf("trakSize=%d StartOffset=%d\n",htonl(buf[0]),offset);
			//offset += htonl(buf[0]);
			{
				//read udta     
				//offset += 8;   	
				//memset(buf,0,sizeof(buf));
				lseek(fd,offset + htonl(buf[0]),SEEK_SET);///////////////////////////////////////////////////////////
				ret = read(fd,(void *)buf,sizeof(buf));
				if (ret)
				{
					if (htonl(buf[1]) == 0X75647461)
					{
						printf("udta Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
						//offset += htonl(buf[0]);
					}
					else
					{
						printf("not find udta  [type=%04x] \n",htonl(buf[1]));
						int i = 0;
						char *p = buf;
						for (i = 0; i < sizeof(buf); i++)
						{
							printf("%02x ",p[i]);
							if (i%16 == 0)
								printf("\n");
						}
						printf("\n\n");							
					}
				}	
				lseek(fd,offset,SEEK_SET);				
			}
		}
		else
		{
			printf("not find trak [type=%d]\n",htonl(buf[1]));		
		}
	}	
	
	//read tkhd  
	offset += 8;
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X746B6864)
		{
			printf("tkhd Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
			printf("not find tkhd  [type=%d]\n",htonl(buf[1]));
	}		
	
	//read edts   
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X65647473)
		{
			printf("edts  Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			//offset += htonl(buf[0]);
		}
		else
			printf("not find edts [type=%d]\n",htonl(buf[1]));
	}	
	
	//read elst   
	offset += 8;	
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X656C7374)
		{
			printf("elst   Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
			printf("not find elst  [type=%d]\n",htonl(buf[1]));
	}	
	
	//read mdia    	
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X6D646961)
		{
			printf("mdia    Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			//offset += htonl(buf[0]);
		}
		else
			printf("not find mdia [type=%d]\n",htonl(buf[1]));
	}	
	
	//read mdhd     
	offset += 8;	
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X6D646864)
		{
			printf("mdhd Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
			printf("not find mdhd  [type=%d]\n",htonl(buf[1]));
	}		
	
	//read hdlr      	
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X68646C72)
		{
			printf("hdlr Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
			printf("not find hdlr  [type=%d]\n",htonl(buf[1]));
	}	

	//read minf       	
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X6D696E66)
		{
			printf("minf Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			//offset += htonl(buf[0]);
		}
		else
			printf("not find minf  [type=%d]\n",htonl(buf[1]));
	}	

	//read vmhd     
	offset += 8;   	
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X766D6864)
		{
			printf("vmhd Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
			printf("not find vmhd  [type=%d]\n",htonl(buf[1]));
	}		
	
	//read dinf      	
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X64696E66)
		{
			printf("dinf Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			//offset += htonl(buf[0]);
		}
		else
			printf("not find dinf  [type=%d]\n",htonl(buf[1]));
	}	
	
	//read dref     
	offset += 8;   	
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X64726566)
		{
			printf("dref Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			//offset += htonl(buf[0]);
		}
		else
			printf("not find dref  [type=%d]\n",htonl(buf[1]));
	}	
	
	//read url     
	offset += 16;   	
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X75726C20)
		{
			printf("url Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
		{
			printf("not find url  [type=%d]\n",htonl(buf[1]));
			int i = 0;
			char *p = buf;
			for (i = 0; i < sizeof(buf); i++)
			{
				printf("%02x ",p[i]);
				if (i%16 == 0)
					printf("\n");
			}
			printf("\n\n");			
		}
	}	
	
	//read stbl     
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X7374626C)
		{
			printf("stbl Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			//offset += htonl(buf[0]);
		}
		else
			printf("not find stbl  [type=%d]\n",htonl(buf[1]));
	}	
	
	//read stsd    
	offset += 8;	
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X73747364)
		{
			printf("stsd Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			//offset += htonl(buf[0]);
		}
		else
			printf("not find stsd  [type=%d]\n",htonl(buf[1]));
	}		
	
	//read avc1     
	offset += 16;	
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X61766331)
		{
			printf("avc1 Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			//offset += htonl(buf[0]);
			
		}
		else
		{
			printf("not find avc1  [type=%d]\n",htonl(buf[1]));
			int i = 0;
			char *p = buf;
			for (i = 0; i < sizeof(buf); i++)
			{
				printf("%02x ",p[i]);
				if (i%16 == 0)
					printf("\n");
			}
			printf("\n\n");			
		}
	}			
	
	//read avcC     
	offset += 8 + 24 + 3 * 4+ 42;	
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X61766343)
		{
			printf("avcC Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
		{
			printf("not find avcC  [type=%d]\n",htonl(buf[1]));
			int i = 0;
			char *p = buf;
			for (i = 0; i < sizeof(buf); i++)
			{
				printf("%02x ",p[i]);
				if (i%16 == 0)
					printf("\n");
			}
			printf("\n\n");			
		}
	}	

	//read stts   
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X73747473)
		{
			printf("stts Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
		{
			printf("not find stts [type=%d]\n",htonl(buf[1]));
			int i = 0;
			char *p = buf;
			for (i = 0; i < sizeof(buf); i++)
			{
				printf("%02x ",p[i]);
				if (i%16 == 0)
					printf("\n");
			}
			printf("\n\n");			
		}
	}	

	//read stss   
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X73747373)
		{
			printf("stss Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
		{
			printf("not find stss [type=%d]\n",htonl(buf[1]));	
		}
	}
	
	//read stsc   
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X73747363)
		{
			printf("stsc Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
		{
			printf("not find stsc [type=%d]\n",htonl(buf[1]));	
		}
	}	
	
	//read stsz   
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X7374737A)
		{
			printf("stsz Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
		{
			printf("not find stsz [type=%d]\n",htonl(buf[1]));	
		}
	}
	
	//read stco  
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X7374636F)
		{
			printf("stco Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
		{
			printf("not find stco [type=%d]\n",htonl(buf[1]));	
		}
	}
	
	//read udta  
	memset(buf,0,sizeof(buf));
	lseek(fd,offset,SEEK_SET);
	ret = read(fd,(void *)buf,sizeof(buf));
	if (ret)
	{
		if (htonl(buf[1]) == 0X75647461)
		{
			printf("udta Size=%d StartOffset=%d\n",htonl(buf[0]),offset);
			offset += htonl(buf[0]);
		}
		else
		{
			printf("not find udta [type=%d]\n",htonl(buf[1]));	
		}
	}	
	close(fd);
}