#include "utility.h"
#include "Tiny.h"
#include "query.h"


using namespace std;

extern queue<int> free_blocks;

int main(int argc, char ** argv){
	//=======================Initialization=========================
	cout << "=======================Initialization=========================" << endl;

	// Initialize the memory, disk and the schema manager
	MainMemory mem;
	Disk disk;
	cout << "The memory contains " << mem.getMemorySize() << " blocks" << endl;
	//cout << mem << endl << endl;
	SchemaManager schema_manager(&mem,&disk);

	disk.resetDiskIOs();
	disk.resetDiskTimer();

	resetFreeBlocks();

	// Another way to time
	clock_t start_time;
	start_time=clock();

	//=======================Read Input=========================
	string line;
	string filename;
	vector<string> words;
	assert(argv && argc >= 1);
	if(argc == 2){
	  filename = argv[1];
	}
	else{
	  cout<<"More than one or no input argument! Read the default test case."<<endl;
	  filename = "TinySQL_linux_updated.txt";
	}
	//ifstream input("TinySQL_linux_updated.txt");
	ifstream input(filename.c_str());
	//ifstream input("test_large.txt");
	if (input.is_open()){
		// for each command line
		while(getline(input, line)){
			//istringstream iss(line);
			if(line[0] == '#')	continue;
			if(line.size() == 0)	continue;
			cout<<line<<endl;	
			// extract each word into vector words
			words = splitBy(line," ");

			// prepare memory
			resetFreeBlocks();
	
			if (words[0] == "CREATE"){
				string relation_name = words[2];
				vector<string> field_names;
				vector<enum FIELD_TYPE> field_types;

				for (int i = 3; i < words.size(); i=i+2){
					field_names.push_back(strip(words[i]));
					if (strip(words[i+1]) == "INT"){
						field_types.push_back(INT);
					}
					else{
						field_types.push_back(STR20);
					}
				}

				Schema schema(field_names,field_types); 

				Relation* relation_ptr=schema_manager.createRelation(relation_name,schema);
				//cout << schema_manager << endl << endl;

			}
			else if (words[0] == "DROP"){
				string relation_name = words[2];
				schema_manager.deleteRelation(relation_name);
			}
			else if (words[0] == "INSERT"){
				Insert(words, line, schema_manager, mem);
			//	cout<< *(schema_manager.getRelation(relation_name))<<endl;
			}
			else if (words[0] == "DELETE"){
				Delete(words, schema_manager, mem);
			}
			else if (words[0] == "SELECT"){
				Select(words, schema_manager, mem);
			}
			else{
				cout<<"SQL command not recognized!"<<endl;
				cout<<line<<endl;
				abort();
			}
			words.clear();
		}
		input.close();
		cout << "Real elapse time = " << ((double)(clock()-start_time)/CLOCKS_PER_SEC*1000) << " ms" << endl;
		cout << "Calculated elapse time = " << disk.getDiskTimer() << " ms" << endl;
		cout << "Calculated Disk I/Os = " << disk.getDiskIOs() << endl;
	}
	else{
	  cout<<"Cannot Open file: "<<filename<<endl;
	}
	return 0;
}


