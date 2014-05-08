#include <iostream>

class leafnode{
public:
	int mass;
	int density;
	
	leafnode(int m=0, int d=0){
		mass = m;
		density = d;
	}
};

class octreenode{
public:
	char bytes[2][2][2]; //0 if leafnode, 1 if octreenode
	void *children[2][2][2]; //typeless pointers
	
	octreenode(int mass=0, int density=0){ //constructor
		for (int x = 0; x < 2; x++){
			for (int y = 0; y < 2; y++){
				for (int z = 0; z < 2; z++){
					bytes[x][y][z] = 0;
					leafnode *temp = new leafnode(mass, density);
					children[x][y][z] = temp;
				}
			}
		}
	}
	
	~octreenode(){ //destructor
		for (int x = 0; x < 2; x++){
			for (int y = 0; y < 2; y++){
				for (int z = 0; z < 2; z++){
					if (bytes[x][y][z] == 0){
						delete static_cast<leafnode*>(children[x][y][z]);
					}
					else{
						delete static_cast<octreenode*>(children[x][y][z]);
					}
				}
			}
		}
	}
	
	void split(int x,int y,int z){ //replaces a leafnode by an octreenode with eight new leafnodes
		if (bytes[x][y][z] != 0) return;
		leafnode *temp = static_cast<leafnode*>(children[x][y][z]);
		int mass = (*temp).mass;
		int density = (*temp).density;
		delete temp;
		octreenode *temp2 = new octreenode(mass,density);
		children[x][y][z] = temp2;
		bytes[x][y][z] = 1;
	}
	
	void refuse(int x,int y,int z){ //replaces an octreenode by a leafnode
		if (bytes[x][y][z] != 1) return;
		octreenode *temp = static_cast<octreenode*>(children[x][y][z]);
		int mass = (*temp).mass();
		int density = (*temp).density();
		delete temp;
		leafnode *temp2 = new leafnode(mass, density);
		children[x][y][z] = temp2;
		bytes[x][y][z] = 0;
	}
	
	int mass(){ //evaluates total mass by adding mass of child nodes
		int totalmass = 0;
		for (int x = 0; x < 2; x++){
			for (int y = 0; y < 2; y++){
				for (int z = 0; z < 2; z++){
					if (bytes[x][y][z] == 0){
						totalmass += (*static_cast<leafnode*>(children[x][y][z])).mass;
					}
					else{
						totalmass += (*static_cast<octreenode*>(children[x][y][z])).mass();
					}
				}
			}
		}
		return totalmass;
	}
	
	int density(){ //evaluates total density by averaging mass of child nodes
		int totaldensity = 0;
		for (int x = 0; x < 2; x++){
			for (int y = 0; y < 2; y++){
				for (int z = 0; z < 2; z++){
					if (bytes[x][y][z] == 0){
						totaldensity += (*static_cast<leafnode*>(children[x][y][z])).density;
					}
					else{
						totaldensity += (*static_cast<octreenode*>(children[x][y][z])).density();
					}
				}
			}
		}
		return totaldensity / 8;
	}
};

int main()
{
	octreenode Galaxy;
	return sizeof(octreenode);
}
