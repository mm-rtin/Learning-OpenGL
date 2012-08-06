
#include <Windows.h>
#include <system/ModelLoader.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <system/Mesh.hpp>
#include <system/Material.h>
#include <system/Math3d.hpp>
#include <system/Globals.hpp>

using namespace std;

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** StrToInt
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
inline int StrToInt(const string &str)
{
	int i;
	if (sscanf(str.c_str(), "%i", &i) == 1)
		return i;
	else return 0;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** split_string
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
inline vector<string> split_string(const string& str, const string& split_str) 
{
	vector<string> stlv_string;
	string part_string("");
	string::size_type i = 0;

	while(i < str.size()) 
	{
		if(split_str.find(str[i]) != string::npos) 
		{
			stlv_string.push_back(part_string);
			part_string = "";
			while(split_str.find(str[i]) != string::npos) 
			{
				++i;
			}
		}
		else 
		{
			part_string += str[i];
			++i;
		}
	}
	if (!part_string.empty())
		stlv_string.push_back(part_string);
	return stlv_string;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** trim
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
inline string trim(const string& str)
{
	string::size_type i = 0;
	while(i < str.size()) 
	{
		if (str[i] == ' ' || str[i] == '\t')
			++i;
		else
			break;
	}
	return str.substr(i);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** load
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Mesh* ModelLoader::load(std::string file) {

	ifstream infile(file);
	if(infile.fail())
	{
		cout<<"Error opening file " << file << endl;
		return false;
	}

	Mesh* mesh = new Mesh;
	mesh->m_mtl.push_back(new Material);

	char current_line[1024];

	while (!infile.eof())
	{
		infile.getline(current_line, 1024);

		string trimmed = trim(current_line);
		if (trimmed.size() == 0)
			continue;
		switch (trimmed[0])
		{
		case 'v':
			{
				float x, y, z;
				switch(trimmed[1])
				{
				case 'n':
					sscanf(trimmed.c_str(), "vn %f %f %f", &x, &y, &z);
					mesh->m_n.push_back(V3f(x, y, z));
					break;

				case 't':
					sscanf(trimmed.c_str(), "vt %f %f", &x, &y);
					mesh->m_texc.push_back(V2f(x, y));
					break;

				default:
					sscanf(trimmed.c_str(), "v %f %f %f", &x, &y, &z);
					mesh->m_v.push_back(V3f(x, y, z));
					mesh->m_vo.push_back(V3f(x, y, z));
					break;
				}
			}
			break;

		case 'f':
			{
				vector<string> faces = split_string(trimmed.c_str(), " \t.\r\n");

				int vnt[3][3] = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};
				string::size_type begin, end;

				for(int i = 0; i < 2; i++)
				{
					begin = 0;
					int j = 0;
					do
					{
						end = faces[i+1].find_first_of("/", begin);

						if(begin < end)
						{
							vnt[i][j] = StrToInt(faces[i+1].substr(begin, end - begin)) - 1;
						}
						else
							vnt[i][j] = -1;
						begin = end+1;
						j++;
					}while(end != string::npos);
				}

				for(unsigned int i = 3; i < faces.size(); i++)
				{
					begin = 0;
					int j = 0;
					do
					{
						end = faces[i].find_first_of("/", begin);

						if(begin < end)
						{
							vnt[2][j] = StrToInt(faces[i].substr(begin, end - begin)) - 1;
						}
						else
							vnt[2][j] = -1;
						begin = end+1;
						j++;

					}while(end != string::npos);

					for(int j = 0; j < 3; j++)
					{
						mesh->m_vi.push_back(vnt[j][0]);

						if(vnt[j][1] != -1) mesh->m_ti.push_back(vnt[j][1]);
						if(vnt[j][2] != -1) mesh->m_ni.push_back(vnt[j][2]);
					}

					memcpy(&vnt[1], &vnt[2], sizeof(int) * 3);
				}
			}
			break;

		case 'm':
			{
				vector<string> tokens = split_string(trimmed, " \t\r\n");
				if(tokens[0] == "mtllib")
				{
					int idx = string(file).find_last_of('/');
					loadMtl((string(file).substr(0, idx + 1) + tokens[1]).c_str(), mesh->m_mtl);
				}
			}
			break;

		case 'u':
			{
				vector<string> tokens = split_string(trimmed, " \t\r\n");
				if(tokens[0] == "usemtl")
				{
					mesh->m_mtlvi.push_back(mesh->m_vi.size());
					// TODO: try stl
					bool findmtl = false;
					for (unsigned int i = 0; i < mesh->m_mtl.size(); i++)
					{
						if (mesh->m_mtl[i]->m_name == tokens[1])
						{
							mesh->m_mtli.push_back(i);
							findmtl = true;
							break;
						}
					}
					if (!findmtl)
						mesh->m_mtli.push_back(0);
				}
			}
			break;
			
		default: break;
		}
	}

	///** GENERATE DEFAULT TEXTURE COORDINATES
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	int triCount = 0;
	int div = 15;
	float xInc = 1.0f / (float)div;
	float yInc = 1.0f / (float)div;
	float tx = 0.0f;
	float ty = 0.0f;

	if (mesh->m_texc.size() == 0) {

		for (int i = 0; i < mesh->m_vi.size(); i++) {

			if (i % div == 0) {
				tx = 0.0f;
				ty += yInc;
			}

			if (i % 3 == 0) {
				if (triCount % 2 == 0)
					mesh->m_texc.push_back(V2f(tx, ty));
				else
					mesh->m_texc.push_back(V2f(tx, ty));

			} else if (i % 3 == 1) {
				if (triCount % 2 == 0)
					mesh->m_texc.push_back(V2f(tx + xInc, ty + yInc));
				else
					mesh->m_texc.push_back(V2f(tx + xInc, ty));

			} else {
				if (triCount % 2 == 0)
					mesh->m_texc.push_back(V2f(tx, ty + yInc));
				else
					mesh->m_texc.push_back(V2f(tx + xInc, ty + yInc));

				tx += xInc;
				triCount++;
			}
			mesh->m_ti.push_back(i);
		}
	}

	return mesh;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** loadMtl
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool ModelLoader::loadMtl(const char* file, vector<Material*>& mat_set) {

	ifstream infile(file);
	if(infile.fail())
	{
		cout << "Error opening file " << file << endl;
		return false;
	}

	char current_line[1024];

	int line = 0;
	int cur_m = -1;

	while (!infile.eof())
	{
		infile.getline(current_line, 1024);
		line ++;
		string trimmed = trim(current_line);
		if (trimmed.size() == 0)
			continue;
		switch (trimmed[0])
		{
		case 'n':
			{
				vector<string> tokens = split_string(trimmed, " \t\r\n");
				if(tokens[0] == "newmtl")
				{
					Material* mat = new Material;
					mat_set.push_back(mat);

					mat->m_name = tokens[1];
					cur_m = mat_set.size() - 1;
				}
			}
			break;

		case 'K':
			{
				float x, y, z;

				switch(trimmed[1])
				{
				case 'a':
					sscanf(trimmed.c_str(), "Ka %f %f %f", &x, &y, &z);					
					mat_set[cur_m]->m_ambient = Color4f(x, y, z, 1.0f);

					break;
				case 'd':
					sscanf(trimmed.c_str(), "Kd %f %f %f", &x, &y, &z);
					mat_set[cur_m]->m_diffuse = Color4f(x, y, z, 1.0f);

					break;
				case 's':
					sscanf(trimmed.c_str(), "Ks %f %f %f", &x, &y, &z);
					mat_set[cur_m]->m_specular = Color4f(x, y, z, 1.0f);

					break;

				case 'e':
					sscanf(trimmed.c_str(), "Ke %f %f %f", &x, &y, &z);
					mat_set[cur_m]->m_emission = Color4f(x, y, z, 1.0f);

					break;

				default:
					break;
				}
			}
			break;

		case 'm':
			{
				vector<string> tokens = split_string(trimmed, " \t\r\n");
				if(tokens[0] == "map_Kd")
				{
					mat_set[cur_m]->m_texture = tokens[1];
				}
			}
			break;

		//case 'l':
		//	{
		//		float power;
		//		sscanf(trimmed.c_str(), "l %f", &power);
		//		mat_set[cur_m]->m_light_power = power;
		//	}
		//	break;

		case 'N':
			{
				switch(trimmed[1])
				{
				case 's':
					{
						float ns;
						sscanf(trimmed.c_str(), "Ns %f", &ns);						
						mat_set[cur_m]->m_shininess = ns;
					}

					break;

				default:
					break;
				}
			}
			break;

		default: break;
		}
	}

	return true;
}