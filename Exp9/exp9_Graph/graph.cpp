// graph.cpp: implementation of the graph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "graph.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

template<class T>
graph<T>::graph( T *array[] )
{

}

template<class T>
graph<T>::~graph()
{

}

template<class T>
void graph<T>::createAdjustedMatrixGraphFromFile( char fileName[] )
{
	FILE* pFile;          //定义文件指针
	char str[1000];         //存放读出一行文本的字符串
	char strTemp[10];      //判断是否注释行
	cellType eWeight;      //边的信息，常为边的权值
	GraphKind graphType;  //图类型枚举变量
	pFile=fopen(fileName,"r");
	if(!pFile)
	{
		printf("错误：文件%s打开失败。\n",fileName);
		return false;
	}
	while(fgets(str,1000,pFile)!=NULL)
	{
		strLTrim(str);     //删除字符串左边空格，这是一个自定义的函数
		if (str[0]=='\n')    //空行，继续读取下一行
			continue;
		strncpy(strTemp,str,2);
		if(strstr(strTemp,"//")!=NULL)  //跳过注释行
			continue;
		else                       //非注释行、非空行，跳出循环
			break;
	}
    //循环结束，str中应该已经是图的标识Graph，判断标识是否正确
	if(strstr(str,"Graph")==NULL)
	{
		printf("错误：打开的文件格式错误！\n");
		fclose(pFile); //关闭文件
		return false;
	}
	//读取图的类型，跳过空行
	while(fgets(str,1000,pFile)!=NULL)
	{
		strLTrim(str);    //删除字符串左边空格，这是一个自定义函数
		if (str[0]=='\n')   //空行，继续读取下一行
			continue;
		strncpy(strTemp,str,2);
		if(strstr(strTemp,"//")!=NULL)  //注释行，跳过，继续读取下一行
			continue;		
		else                       //非空行，也非注释行，即图的类型标识
			break;
	}
    //设置图的类型
	if(strstr(str,"UDG"))
		graphType=UDG;    //无向图
	else if(strstr(str,"UDN"))
		graphType=UDN;    //无向网
	else if(strstr(str,"DG"))
		graphType=DG;     //有向图
	else if(strstr(str,"DN"))
		graphType=DN;     //有向网
	else
	{
		printf("错误：读取图的类型标记失败！\n");
		fclose(pFile);       //关闭文件
		return false;
	}
	//读取顶点行数据到str。跳过空行
	while(fgets(str,1000,pFile)!=NULL)
	{
		strLTrim(str);      //删除字符串左边空格，这是一个自定义函数
		if (str[0]=='\n')     //空行，继续读取下一行
			continue;
		strncpy(strTemp,str,2);
		if(strstr(strTemp,"//")!=NULL)  //注释行，跳过，继续读取下一行
			continue;		
		else                       //非空行，也非注释行，即图的顶点元素行
			break;
	}

    //顶点数据放入图的顶点数组	
	char* token=strtok(str," ");
	int nNum=0;	
	while(token!=NULL)
	{
		G.Data[nNum]=*token; 
      token = strtok( NULL, " ");
		nNum++;
	}
	//图的邻接矩阵初始化
	int nRow=0;    //矩阵行下标
	int nCol=0;     //矩阵列下标
	if(graphType==UDG || graphType==DG)
	{
		for(nRow=0;nRow<nNum;nRow++)
			for(nCol=0;nCol<nNum;nCol++)
				G.AdjMatrix[nRow][nCol]=0;
	}
	else
	{
		for(nRow=0;nRow<nNum;nRow++)
			for(nCol=0;nCol<nNum;nCol++)
				G.AdjMatrix[nRow][nCol]=INF;  //INF表示无穷大
	}
		//循环读取边的数据到邻接矩阵
	int edgeNum=0;         //边的数量
	elementType Nf, Ns;     //边或弧的2个相邻顶点
	while(fgets(str,1000,pFile)!=NULL)
	{
		strLTrim(str);       //删除字符串左边空格，这是一个自定义函数
		if (str[0]=='\n')      //空行，继续读取下一行
			continue;
		strncpy(strTemp,str,2);
		if(strstr(strTemp,"//")!=NULL)  //注释行，跳过，继续读取下一行
			continue;
		char* token=strtok(str," ");  //以空格为分隔符，分割一行数据，写入邻接矩阵
		if(token==NULL)         //分割为空串，失败退出
		{
			printf("错误：读取图的边数据失败！\n");
			fclose(pFile);         //关闭文件
			return false;
		}
		Nf=*token;               //获取边的第一个顶点
		token = strtok( NULL, " ");   //读取下一个子串，即第二个顶点
		if(token==NULL)          //分割为空串，失败退出
		{
			printf("错误：读取图的边数据失败！\n");
			fclose(pFile);          //关闭文件
			return false;
		}
		Ns=*token;  //获取边的第二个顶点
                 //从第一个顶点获取行号		
		for(nRow=0;nRow<nNum;nRow++)
		{
			if(G.Data[nRow]==Nf)  //从顶点列表找到第一个顶点的编号
				break;
		}
                  //从第二个顶点获取列号
		for(nCol=0;nCol<nNum;nCol++)
		{
			if(G.Data[nCol]==Ns)  //从顶点列表找到第二个顶点的编号
				break;
		}
		//如果为网，读取权值
		if(graphType==UDN || graphType==DN)
		{                //读取下一个子串，即边的附加信息，常为边的权重
			token = strtok( NULL, " ");  
			if(token==NULL)    //分割为空串，失败退出
			{
				printf("错误：读取图的边数据失败！\n");
				fclose(pFile);    //关闭文件
				return false;
			}
			eWeight=atoi(token);  //取得边的附加信息
		}
		if(graphType==UDN || graphType==DN)  
			G.AdjMatrix[nRow][nCol]=eWeight;
//如果为网，邻接矩阵中对应的边设置权值，否则置为1
		else
			G.AdjMatrix[nRow][nCol]=1;  
		edgeNum++;   //边数加1
	}
  G.VerNum=nNum;           //图的顶点数
	if(graphType==UDG || graphType==UDN)
		G.ArcNum=edgeNum / 2;  //无向图或网的边数等于统计的数字除2  
	else
		G.ArcNum=edgeNum;
	G.gKind=graphType;         //图的类型
	fclose(pFile);               //关闭文件
	//return true;

}