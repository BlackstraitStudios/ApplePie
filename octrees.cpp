#include <iostream>
#include <math.h>

int nodesmade = 0;
int leavesmade = 0;

class leafnode
{
public:
	int density;

	leafnode(int d=0) {
		density = d;
		leavesmade++;
	}
};

class octreenode
{
public:
	char bytes[2][2][2]; //0 if leafnode, 1 if octreenode
	void *children[2][2][2]; //typeless pointers

	octreenode(int density=0) { //constructor
		nodesmade++;
		for (int x = 0; x < 2; x++) {
			for (int y = 0; y < 2; y++) {
				for (int z = 0; z < 2; z++) {
					bytes[x][y][z] = 0;
					leafnode *temp = new leafnode(density);
					children[x][y][z] = temp;
				}
			}
		}
	}

	~octreenode() { //destructor
		for (int x = 0; x < 2; x++) {
			for (int y = 0; y < 2; y++) {
				for (int z = 0; z < 2; z++) {
					if (bytes[x][y][z] == 0) {
						delete (leafnode*)children[x][y][z];
					} else {
						delete (octreenode*)(children[x][y][z]);
					}
				}
			}
		}
	}

	void split(int x,int y,int z) { //replaces a leafnode by an octreenode with eight new leafnodes
		if (bytes[x][y][z] != 0) return;
		leafnode *temp = (leafnode*)children[x][y][z];
		int density = temp->density;
		delete temp;
		octreenode *temp2 = new octreenode(density);
		children[x][y][z] = temp2;
		bytes[x][y][z] = 1;
	}

	void refuse(int x,int y,int z) { //replaces an octreenode by a leafnode
		if (bytes[x][y][z] != 1) return;
		octreenode *temp = (octreenode*)children[x][y][z];
		int density = temp->density();
		delete temp;
		leafnode *temp2 = new leafnode(density);
		children[x][y][z] = temp2;
		bytes[x][y][z] = 0;
	}

	int density() { //evaluates total density by averaging density of child nodes
		int totaldensity = 0;
		for (int x = 0; x < 2; x++) {
			for (int y = 0; y < 2; y++) {
				for (int z = 0; z < 2; z++) {
					if (bytes[x][y][z] == 0) {
						totaldensity += ((leafnode*)children[x][y][z])->density;
					} else {
						totaldensity += ((octreenode*)children[x][y][z])->density();
					}
				}
			}
		}
		return totaldensity / 8;
	}

	int getdensity(unsigned int x, unsigned int y, unsigned int z, int depth) { //reads density at specified coordinates
		octreenode *pointer = this;
		for(int n = 0; n < depth; n++) {
			if (pointer->bytes[x>>31][y>>31][z>>31] == 0) return ((leafnode*)pointer->children[x>>31][y>>31][z>>31])->density;
			pointer = (octreenode*)pointer->children[x>>31][y>>31][z>>31];
			x <<= 1;
			y <<= 1;
			z <<= 1;
		}
		return pointer->density();
	}

	void setdensity(unsigned int x, unsigned int y, unsigned int z, int depth, int newdensity) { //alters tree so that a leafnode exists at speficied coordinates and depth and sets density
		octreenode *pointer = this;
		for(int n = 0; n < depth; n++) {
			if (pointer->bytes[x>>31][y>>31][z>>31] == 0) pointer->split(x>>31,y>>31,z>>31);
			pointer = (octreenode*)pointer->children[x>>31][y>>31][z>>31];
			x <<= 1;
			y <<= 1;
			z <<= 1;
		}
		pointer->refuse(x>>31,y>>31,z>>31);
		((leafnode*)pointer->children[x>>31][y>>31][z>>31])->density = newdensity;
	}
};

int main()
{
	octreenode Galaxy(1);
	for(unsigned int x = 0; x<= 536870912; x+= 4096) {
		Galaxy.setdensity(x,0,0,20,500000000);
	}
	return leavesmade;
}
