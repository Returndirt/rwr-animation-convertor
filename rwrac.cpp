// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


using namespace std;

//get files we want 
struct FilesTODO{
	string name;
}fileLine[2000];
int endOfFileLine;
int getfileline(int choice)
{
	endOfFileLine = 0;
	char buffer[BigNumber];
	_getcwd(buffer, BigNumber);
	const char *buff3 = "\\*.dae";
	const char *buff2 = "\\*.xml";
	if (choice == 1)
	{
		strcat_s(buffer, buff2);
	}
	else if (choice == 2)
	{
		strcat_s(buffer, buff3);
	}
	std::string inPath = buffer;
	long handle;
	struct _finddata_t fileinfo;
	handle = _findfirst(inPath.c_str(), &fileinfo);
	if (handle == -1)
		return -1;
	do
	{
		fileLine[++endOfFileLine].name = fileinfo.name;
	} while (!_findnext(handle, &fileinfo));
	_findclose(handle);
	return 0;
}

//matrix algorithm
struct MATRIX{
	double in[5][5];
};
MATRIX Matrix_X(MATRIX a, MATRIX b)
{
	MATRIX c;
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			c.in[i][j] = 0.0;
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			for (int step = 1; step <= 4; step++)
				c.in[i][j] += a.in[i][step] * b.in[step][j];
	return c;
}
MATRIX Make_Matrix(double p11, double p12, double p13, double p14, double p21, double p22, double p23, double p24, double p31, double p32, double p33, double p34, double p41, double p42, double p43, double p44)
{
	MATRIX c;
	c.in[1][1] = p11;
	c.in[1][2] = p12;
	c.in[1][3] = p13;
	c.in[1][4] = p14;
	c.in[2][1] = p21;
	c.in[2][2] = p22;
	c.in[2][3] = p23;
	c.in[2][4] = p24;
	c.in[3][1] = p31;
	c.in[3][2] = p32;
	c.in[3][3] = p33;
	c.in[3][4] = p34;
	c.in[4][1] = p41;
	c.in[4][2] = p42;
	c.in[4][3] = p43;
	c.in[4][4] = p44;
	return c;
}
void printMatrix(MATRIX a)
{
	for (int i = 1; i <= 4; i++)
	{
		for (int j = 1; j <= 4; j++)
		{
			printf("%.5f ", a.in[i][j]);
		}
		printf("\n");
	}
	return;
}

//x-y-z pos
struct XYZ{
	double x;
	double y;
	double z;
};
XYZ makeXYZ(double x, double y, double z)
{
	XYZ a;
	a.x = x;
	a.y = y;
	a.z = z;
	return a;
}
XYZ XYZ_sf(XYZ a,double bl)
{
	return makeXYZ(a.x*bl, a.y*bl, a.z*bl);
}
XYZ XYZ_add(XYZ a, XYZ b)
{
	return makeXYZ(a.x + b.x, a.y + b.y, a.z + b.z);
}

//double to string
string doubleToString(double num)
{
	char str[256];
	sprintf_s(str, "%lf", num);
	string result = str;
	return result;
}
double stringToDouble(string num)
{
	bool minus = false;
	string real = num; 
	if (num.at(0) == '-')
	{
		minus = true;
		real = num.substr(1, num.size() - 1);
	}

	char c;
	int i = 0;
	double result = 0.0, dec = 10.0;
	bool isDec = false;
	unsigned long size = real.size();
	while (i < size)
	{
		c = real.at(i);
		if (c == '.')
		{
			isDec = true;
			i++;
			continue;
		}
		if (!isDec)
		{
			result = result * 10 + c - '0';
		}
		else
		{
			result = result + (c - '0') / dec;
			dec *= 10;
		}
		i++;
	}

	if (minus == true) {
		result = -result;
	}

	return result;
}



int goal;
int totallyNumbers;
double times[BigNumber];
MATRIX animeInPut[25][BigNumber];//nood,time
XYZ animeOutPut[25][BigNumber];///nood,time
XYZ xmlAnime[25][BigNumber];


//get time line
void gettime(string docname)
{
	TiXmlDocument doc( docname.c_str() );
	doc.LoadFile();
	TiXmlElement *root = doc.RootElement();
	TiXmlElement *name = root->FirstChildElement();
	while (name != NULL)
	{
		if ((string)(name->Value()) == "library_animations")
		{
			TiXmlElement *nood = name->FirstChildElement();
			TiXmlElement *source = nood->FirstChildElement();
			TiXmlElement *numbern = source->FirstChildElement();
			const char *cha = numbern->Attribute("count");
			totallyNumbers = atoi(cha);
			string timesString = numbern->GetText();
			//				cout << totallyNumbers << endl;
			//				cout << nood->Value() << endl;
			//				cout << source->Value() << endl;
			//				cout << numbern->Value() << endl;
			//				cout << timesString << endl;
			stringstream cinfromstring(timesString);
			for (int i = 1; i <= totallyNumbers; i++)
			{
				cinfromstring >> times[i];
			}
//			for (int i = 1; i <= totallyNumbers; i++)
//			{
//				cout << times[i] << endl;
//			}
//			break;
		}
		name = name->NextSiblingElement();
	}
}

//get animition matrix
void getmatrix(int noodname,string strtomatrix)
{
	stringstream cinfromstring(strtomatrix);
	double p[5][5];
	for (int step = 1; step <= totallyNumbers; step++)
	{
		for (int i = 1; i <= 4; i++)
		{
			for (int j = 1; j <= 4; j++)
			{
				cinfromstring >> p[i][j];
			}
		}
		animeInPut[noodname][step] = Make_Matrix(p[1][1],p[1][2], p[1][3], p[1][4], p[2][1], p[2][2], p[2][3], p[2][4], p[3][1], p[3][2], p[3][3], p[3][4], p[4][1], p[4][2], p[4][3], p[4][4]);
	
//		printMatrix(animeInPut[noodname][step]);
//		printf("\n");
	
	}
}
void getanimeinput(string docname)
{
	TiXmlDocument doc(docname.c_str());
	doc.LoadFile();
	TiXmlElement *root = doc.RootElement();
	TiXmlElement *name = root->FirstChildElement();
	while (name != NULL)
	{
		if ((string)(name->Value()) == "library_animations")
		{
			TiXmlElement *nood = name->FirstChildElement();
			while (nood != NULL)
			{
				const char *cha = nood->Attribute("id");
				if ((string)cha == "mixamorig_Hips-anim")//1
				{
			//		cout << "1" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(1, lsstr);
				}
				else if ((string)cha == "mixamorig_Spine-anim")//2
				{
			//		cout << "2" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(2, lsstr);
				}
				else if ((string)cha == "mixamorig_Spine1-anim")//3
				{
			//		cout << "3" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(3, lsstr);
				}
				else if ((string)cha == "mixamorig_Spine2-anim")//4
				{
			//		cout << "4" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(4, lsstr);
				}
				else if ((string)cha == "mixamorig_Neck-anim")//5
				{
			//		cout << "5" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(5, lsstr);
				}
				else if ((string)cha == "mixamorig_Head-anim")//6
				{
			//		cout << "6" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(6, lsstr);
				}
				else if ((string)cha == "mixamorig_LeftShoulder-anim")//7
				{
			//		cout << "7" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(7, lsstr);
				}
				else if ((string)cha == "mixamorig_LeftArm-anim")//8
				{
			//		cout << "8" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(8, lsstr);
				}
				else if ((string)cha == "mixamorig_LeftForeArm-anim")//9
				{
			//		cout << "9" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(9, lsstr);
				}
				else if ((string)cha == "mixamorig_LeftHand-anim")//10
				{
			//		cout << "10" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(10, lsstr);
				}
				else if ((string)cha == "mixamorig_RightShoulder-anim")//11
				{
			//		cout << "11" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(11, lsstr);
				}
				else if ((string)cha == "mixamorig_RightArm-anim")//12
				{
			//		cout << "12" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(12, lsstr);
				}
				else if ((string)cha == "mixamorig_RightForeArm-anim")//13
				{
			//		cout << "13" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(13, lsstr);
				}
				else if ((string)cha == "mixamorig_RightHand-anim")//14
				{
			//		cout << "14" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(14, lsstr);
				}
				else if ((string)cha == "mixamorig_LeftUpLeg-anim")//15
				{
			//		cout << "15" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(15, lsstr);
				}
				else if ((string)cha == "mixamorig_LeftLeg-anim")//16
				{
			//		cout << "16" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(16, lsstr);
				}
				else if ((string)cha == "mixamorig_LeftFoot-anim")//17
				{
			//		cout << "17" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(17, lsstr);
				}
				else if ((string)cha == "mixamorig_RightUpLeg-anim")//18
				{
			//		cout << "18" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(18, lsstr);
				}
				else if ((string)cha == "mixamorig_RightLeg-anim")//19
				{
			//		cout << "19" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(19, lsstr);
				}
				else if ((string)cha == "mixamorig_RightFoot-anim")//20
				{
			//		cout << "20" << endl;
					TiXmlElement *lsnood = nood;
					lsnood = lsnood->FirstChildElement();
					lsnood = lsnood->NextSiblingElement();
					lsnood = lsnood->FirstChildElement();
					string lsstr = lsnood->GetText();
					getmatrix(20, lsstr);
				}

				
				nood = nood->NextSiblingElement();
			}
		}
		name = name->NextSiblingElement();
	}
}

//translate matrix anime to xyz anime
void transanime()
{
	for (int step = 1; step <= totallyNumbers; step++)
	{
		MATRIX WorldMatrix = animeInPut[1][step];
		MATRIX HipMatrix = WorldMatrix;
		animeOutPut[1][step] = makeXYZ(WorldMatrix.in[1][4], WorldMatrix.in[2][4], WorldMatrix.in[3][4]);
		//
		//printf("id:%d x:%.5f y:%.5f z:%.5f\n", 1, animeOutPut[1][step].x, animeOutPut[1][step].y, animeOutPut[1][step].z);
		//
		for (int i = 2; i <= 5; i++)
		{
			WorldMatrix = Matrix_X(WorldMatrix, animeInPut[i][step]);
			animeOutPut[i][step] = makeXYZ(WorldMatrix.in[1][4], WorldMatrix.in[2][4], WorldMatrix.in[3][4]);
			//
			//printf("id:%d x:%.5f y:%.5f z:%.5f\n",i, animeOutPut[i][step].x, animeOutPut[i][step].y, animeOutPut[i][step].z);
			//
		}
		MATRIX NeckMatrix = WorldMatrix;
		WorldMatrix = Matrix_X(WorldMatrix, animeInPut[6][step]);
		animeOutPut[6][step] = makeXYZ(WorldMatrix.in[1][4], WorldMatrix.in[2][4], WorldMatrix.in[3][4]);
		WorldMatrix = NeckMatrix;
		for (int i = 7; i <= 10; i++)
		{
			WorldMatrix = Matrix_X(WorldMatrix, animeInPut[i][step]);
			animeOutPut[i][step] = makeXYZ(WorldMatrix.in[1][4], WorldMatrix.in[2][4], WorldMatrix.in[3][4]);
			//
			//printf("id:%d x:%.5f y:%.5f z:%.5f\n", i, animeOutPut[i][step].x, animeOutPut[i][step].y, animeOutPut[i][step].z);
			//
		}
		WorldMatrix = NeckMatrix;
		for (int i = 11; i <= 14; i++)
		{
			WorldMatrix = Matrix_X(WorldMatrix, animeInPut[i][step]);
			animeOutPut[i][step] = makeXYZ(WorldMatrix.in[1][4], WorldMatrix.in[2][4], WorldMatrix.in[3][4]);
			//
			//printf("id:%d x:%.5f y:%.5f z:%.5f\n", i, animeOutPut[i][step].x, animeOutPut[i][step].y, animeOutPut[i][step].z);
			//
		}
		WorldMatrix = HipMatrix;
		for (int i = 15; i <= 17; i++)
		{
			WorldMatrix = Matrix_X(WorldMatrix, animeInPut[i][step]);
			animeOutPut[i][step] = makeXYZ(WorldMatrix.in[1][4], WorldMatrix.in[2][4], WorldMatrix.in[3][4]);
			//
			//printf("id:%d x:%.5f y:%.5f z:%.5f\n", i, animeOutPut[i][step].x, animeOutPut[i][step].y, animeOutPut[i][step].z);
			//
		}
		WorldMatrix = HipMatrix;
		for (int i = 18; i <= 20; i++)
		{
			WorldMatrix = Matrix_X(WorldMatrix, animeInPut[i][step]);
			animeOutPut[i][step] = makeXYZ(WorldMatrix.in[1][4], WorldMatrix.in[2][4], WorldMatrix.in[3][4]);
			//
			//printf("id:%d x:%.5f y:%.5f z:%.5f\n", i, animeOutPut[i][step].x, animeOutPut[i][step].y, animeOutPut[i][step].z);
			//
		}
	}
	for (int step = 1; step <= totallyNumbers; step++)
	{
		xmlAnime[1][step] = animeOutPut[6][step];
		xmlAnime[2][step] = animeOutPut[5][step];
		xmlAnime[3][step] = animeOutPut[12][step];
		xmlAnime[4][step] = animeOutPut[8][step];
		xmlAnime[5][step] = animeOutPut[13][step];
		xmlAnime[6][step] = animeOutPut[9][step];
		xmlAnime[7][step] = animeOutPut[14][step];
		xmlAnime[8][step] = animeOutPut[10][step];
		xmlAnime[9][step] = animeOutPut[3][step];//
		xmlAnime[10][step] = animeOutPut[18][step];
		xmlAnime[11][step] = animeOutPut[15][step];
		xmlAnime[12][step] = animeOutPut[19][step];
		xmlAnime[13][step] = animeOutPut[16][step];
		xmlAnime[14][step] = animeOutPut[20][step];
		xmlAnime[15][step] = animeOutPut[17][step];
	}

	return;
}

//putout xml anime
void putxmlOut(string filename,double bl)
{
	filename = filename+".xml";
	fstream coutfile;
	coutfile.open(filename,ios::out);
	coutfile << "<animations>" << endl;
	coutfile << " <animation loop=\""<<1<<"\" end=\""<<times[totallyNumbers]<< "\" speed=\""<<1.000<<"\" comment=\""<<filename<<"\">"<< endl;
	for (int i = 1; i <= totallyNumbers; i++)
	{
		coutfile << "  <frame time = \""<<times[i]<<"\">" << endl;
		for (int j = 1; j <= 15; j++)
		{
			coutfile << "   <position x=\"" << xmlAnime[j][i].x*bl << "\" y=\"" << xmlAnime[j][i].y*bl << "\" z=\"" << xmlAnime[j][i].z*bl << "\" />" << endl;
		}
		coutfile << "  </frame>" << endl;
	}
	coutfile << " </animation>" << endl;
	coutfile << "</animations>" << endl;
}

//rwrhua
void rwrhua()
{
	XYZ ls;
	for (int i = 1; i <= totallyNumbers; i++)
	{
		//head neck
		ls = XYZ_add(xmlAnime[1][i], XYZ_sf(xmlAnime[2][i], -1.0));
		xmlAnime[1][i] = XYZ_add(xmlAnime[1][i], XYZ_sf(ls, 0.8));
		xmlAnime[2][i] = XYZ_add(xmlAnime[2][i], XYZ_sf(ls, 0.8));
		//hip
		ls = XYZ_add(xmlAnime[11][i], XYZ_sf(xmlAnime[10][i], -1.0));
		xmlAnime[11][i] = XYZ_add(xmlAnime[11][i], XYZ_sf(ls, 0.4));
		xmlAnime[10][i] = XYZ_add(xmlAnime[10][i], XYZ_sf(ls, -0.4));
		//leg
		ls = XYZ_add(xmlAnime[13][i], XYZ_sf(xmlAnime[12][i], -1.0));
		xmlAnime[13][i] = XYZ_add(xmlAnime[13][i], XYZ_sf(ls, 0.2));
		xmlAnime[12][i] = XYZ_add(xmlAnime[12][i], XYZ_sf(ls, -0.2));
		//hright body
		ls = XYZ_add(xmlAnime[3][i], XYZ_sf(xmlAnime[10][i], -1.0));
		xmlAnime[3][i] = XYZ_add(xmlAnime[3][i], XYZ_sf(ls, -0.1));
		
		ls = XYZ_add(xmlAnime[4][i], XYZ_sf(xmlAnime[11][i], -1.0));
		xmlAnime[4][i] = XYZ_add(xmlAnime[4][i], XYZ_sf(ls, -0.1));
		//shelder
		ls = XYZ_add(xmlAnime[3][i], XYZ_sf(xmlAnime[4][i], -1.0));
		xmlAnime[3][i] = XYZ_add(xmlAnime[3][i], XYZ_sf(ls, 0.0));
		xmlAnime[4][i] = XYZ_add(xmlAnime[4][i], XYZ_sf(ls, -0.0));
		//leg
		ls = XYZ_add(xmlAnime[10][i], XYZ_sf(xmlAnime[12][i], -1.0));
		xmlAnime[12][i] = XYZ_add(xmlAnime[12][i], XYZ_sf(ls, 0.1));

		ls = XYZ_add(xmlAnime[11][i], XYZ_sf(xmlAnime[13][i], -1.0));
		xmlAnime[13][i] = XYZ_add(xmlAnime[13][i], XYZ_sf(ls, 0.1));
		//arm
		ls = XYZ_add(xmlAnime[5][i], XYZ_sf(xmlAnime[3][i], -1.0));
		xmlAnime[5][i] = XYZ_add(xmlAnime[5][i], XYZ_sf(ls, 0.2));

		ls = XYZ_add(xmlAnime[6][i], XYZ_sf(xmlAnime[4][i], -1.0));
		xmlAnime[6][i] = XYZ_add(xmlAnime[6][i], XYZ_sf(ls, 0.2));

		ls = XYZ_add(xmlAnime[7][i], XYZ_sf(xmlAnime[3][i], -1.0));
		xmlAnime[7][i] = XYZ_add(xmlAnime[7][i], XYZ_sf(ls, 0.2));

		ls = XYZ_add(xmlAnime[8][i], XYZ_sf(xmlAnime[4][i], -1.0));
		xmlAnime[8][i] = XYZ_add(xmlAnime[8][i], XYZ_sf(ls, 0.2));
	}
}

//input xml
void xmlscale(string docname,double scale)
{
	TiXmlDocument doc(docname.c_str());
	doc.LoadFile();
	TiXmlElement *root = doc.RootElement();
	if ((string)root->Value() != "animations")return;
	TiXmlElement *name = root->FirstChildElement();
	while (name != NULL)
	{
		TiXmlElement *nood = name->FirstChildElement();
		while (nood != NULL)
		{
			TiXmlElement *anime = nood->FirstChildElement();
			while (anime != NULL)
			{
//				cout << anime->Value() << endl;
				const char * x;
				const char * y;
				const char * z;
				x = anime->Attribute("x");
				y = anime->Attribute("y");
				z = anime->Attribute("z");
				anime->SetAttribute("x", doubleToString(stringToDouble(x)*scale).c_str());
				anime->SetAttribute("y", doubleToString(stringToDouble(y)*scale).c_str());
				anime->SetAttribute("z", doubleToString(stringToDouble(z)*scale).c_str());

				anime = anime->NextSiblingElement();
			}
			nood = nood->NextSiblingElement();
		}
		name = name->NextSiblingElement();
	}
	doc.SaveFile();
}

//cutframe
void cutframe(int x)
{
	for (int i = 2; i < totallyNumbers / x; i++)
	{
		times[i] = times[i*x];
		for (int j = 1; j <= 20; j++)
		{
			animeInPut[j][i] = animeInPut[j][i*x];
		}
	}
	times[totallyNumbers/x] = times[totallyNumbers];
	for (int j = 1; j <= 20; j++)
	{
		animeInPut[j][totallyNumbers / x] = animeInPut[j][totallyNumbers];
	}
	totallyNumbers = totallyNumbers / x;
}


int main()
{
	printf("Author: Square\nContributors: Xe-No , PlayerFox 69\n");
	printf("1.Convert dae files downloaded directly from mixamo  导入并转化mixamo默认dae文件 \n");
	printf("2.Convert dae files bound on mixamo		     导入并转化绑定过rwr模型的mixamo dae文件 \n");
	printf("3.Convert dae files directly to xml	             导入并原样转化mixamo dae文件 \n");
	printf("4.Scale xml animations	                             批量缩放xml骨骼大小 \n");
	printf("Enter a number:");
	scanf_s("%d", &goal);
	/*
	if (goal == 0)//test
	{
		getfileline(2);
		for (int i = 1; i <= endOfFileLine; i++)
		{
			cout << fileLine[i].name << endl;
			gettime(fileLine[i].name);
			getanimeinput(fileLine[i].name);
			transanime();
//			cout << "trans ok" << endl;
			rwrhua();
			putxmlOut(fileLine[i].name,0.3);
			
		}
	}*/
	 if (goal==1)
	{
		int bl;
		printf("Frame rate conversion rate 请输入缩减帧数的倍数(一个整数）:");
		cin >> bl;

		getfileline(2);
		for (int i = 1; i <= endOfFileLine; i++)
		{
			cout << fileLine[i].name << endl;
			gettime(fileLine[i].name);
			getanimeinput(fileLine[i].name);
			cutframe(bl);
			transanime();
			rwrhua();
			putxmlOut(fileLine[i].name, 0.3);
		}
	}
	else if (goal==2)
	{
		int bl;
		printf("Frame rate conversion rate 请输入缩减帧数的倍数(一个整数）:");
		cin >> bl;
		getfileline(2);
		for (int i = 1; i <= endOfFileLine; i++)
		{
			cout << fileLine[i].name << endl;
			gettime(fileLine[i].name);
			getanimeinput(fileLine[i].name);
			cutframe(bl);
			transanime();
			rwrhua();
			putxmlOut(fileLine[i].name, 9.0);
		}
	}
	else if (goal == 3)
	{
		int bl;
		printf("Frame rate conversion rate 请输入缩减帧数的倍数(一个整数）:");
		cin >> bl;
		getfileline(2);
		for (int i = 1; i <= endOfFileLine; i++)
		{
			cout << fileLine[i].name << endl;
			gettime(fileLine[i].name);
			getanimeinput(fileLine[i].name);
			cutframe(bl);
			transanime();
			putxmlOut(fileLine[i].name, 1.0);
		}
	}
	else if (goal == 4)
	{
		double scaleNum;
		getfileline(1);
		printf("Enter the multiplier you need:");
		cin >> scaleNum;
		for (int i = 1; i <= endOfFileLine; i++)
		{
			cout << fileLine[i].name << endl;
			xmlscale(fileLine[i].name, scaleNum);
		}
	}


	system("pause");
	return 0;
}