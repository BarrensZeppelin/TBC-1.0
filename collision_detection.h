/*
	*	Tile Based Collision Detection
	*
	*	Written and edited by:
	*		Oskar Haarklou Veileborg
	*	2012	ohv1020@hotmail.com
	*
*/

#pragma once

#include <vector>
#include <string>
#include <time.h>
using namespace std;

namespace tbc {
	class cdTile {
	private:
		int tX;
		int tY;

		vector<string *> occupiers;
		vector<int> occupierIDs;

	public:
		cdTile(int x, int y) : tX(x), tY(y) {}
		cdTile() : tX(0), tY(0) {}
		~cdTile() {
			for(unsigned int i=0; i<occupiers.size(); i++) {
				delete occupiers[i];
			}
		}
		
		void setTaken(string * sTakenBy, int id) {
			occupiers.push_back(sTakenBy);
			occupierIDs.push_back(id);
		}
		
		void unset(string sTakenBy, int id) {
			for(unsigned int i = 0; i<occupiers.size(); i++) {
				if(occupierIDs[i]==id || id == -1) { //if id is -1, we don't care about it
					if(occupiers[i]->compare(sTakenBy)==0) { 
						delete occupiers[i];
						occupiers.erase(occupiers.begin()+i);
						occupierIDs.erase(occupierIDs.begin()+i);
						i-=1;
					}
				}
			}
		}
		

		void setValues(int xPos, int yPos) {
			tX = xPos;
			tY = yPos;
		}

		// ------------ Getters ----------------
		int GetX()							{return tX;			}
		int GetY()							{return tY;			}

		bool GetTaken()						{return !occupiers.empty();}
		bool GetTaken(string check) {
			for(unsigned int i = 0; i<occupiers.size(); i++) {
				if(occupiers[i]->compare(check)==0) {
					return true;
				}	
			}
		}
	};

	vector<vector<cdTile> > tiles;

	int tileSize = 8; //Size of tiles defaults to 8, this can be changed at any time.



	void initCollisionTiles(int areaWidth, int areaHeight) {
		tiles.clear();

		unsigned int xTiles = areaWidth/tileSize;
		unsigned int yTiles = areaHeight/tileSize;

		tiles.resize(xTiles);
		
		unsigned long begTime = clock();
		for(unsigned int i=0; i<xTiles; i++) {
			tiles[i].resize(yTiles);
			for(unsigned int y=0; y<yTiles; y++) {
				tiles[i][y] = cdTile(i*tileSize, y*tileSize);
			}
		}
		cout << "Creation of Collision Tile Array took " << (unsigned long) (clock() - begTime) / CLOCKS_PER_SEC << "seconds"<< endl;
	}


	/////////////////////////////////////////////////
	//--------Actual collsion checking-------------//
	/////////////////////////////////////////////////

	bool CheckCollision(int minX, int minY, int maxX, int maxY, string sOccupier) {
		int posXTiles;
		if((maxX-minX)%tileSize==0) {posXTiles = (maxX-minX)/tileSize;}
		else {posXTiles = ((maxX-minX)+tileSize)/tileSize;}

		
		int posYTiles;
		if((maxY-minY)%tileSize==0) {posYTiles = (maxY-minY)/tileSize;}
		else {posYTiles = ((maxY-minY)+tileSize)/tileSize;}
		
		bool collision = false;

		for(int i = (minX-(minX%tileSize))/tileSize; i < (minX-(minX%tileSize)+posXTiles*tileSize)/tileSize; i++) {
			for(int u = (minY-(minY%tileSize))/tileSize; u < (minY-(minY%tileSize)+posYTiles*tileSize)/tileSize; u++) {
				if(tiles[i][u].GetTaken(sOccupier)) {
					collision = true;
				}
			}
		}
		
		return collision;
	}

	bool CheckCollision(int minX, int minY, int maxX, int maxY) {
		int posXTiles;
		if((maxX-minX)%tileSize==0) {posXTiles = (maxX-minX)/tileSize;}
		else {posXTiles = ((maxX-minX)+tileSize)/tileSize;}

			
		int posYTiles;
		if((maxY-minY)%tileSize==0) {posYTiles = (maxY-minY)/tileSize;}
		else {posYTiles = ((maxY-minY)+tileSize)/tileSize;}
		
		bool collision = false;

		for(int i = (minX-(minX%tileSize))/tileSize; i < (minX-(minX%tileSize)+posXTiles*tileSize)/tileSize; i++) {
			for(int u = (minY-(minY%tileSize))/tileSize; u < (minY-(minY%tileSize)+posYTiles*tileSize)/tileSize; u++) {
				if(tiles[i][u].GetTaken()) {
					collision = true;
				}
			}
		}
		
		return collision;
	}


	vector<cdTile *> CollisionArray(int minX, int minY, int maxX, int maxY, bool OnlyReturnTaken) {
		vector<cdTile *> collisionArray;
		
		int posXTiles;
		if((maxX-minX)%tileSize==0) {posXTiles = (maxX-minX)/tileSize;}
		else {posXTiles = ((maxX-minX)+tileSize)/tileSize;}
			
		int posYTiles;
		if((maxY-minY)%tileSize==0) {posYTiles = (maxY-minY)/tileSize;}
		else {posYTiles = ((maxY-minY)+tileSize)/tileSize;}

		for(int i = (minX-(minX%tileSize))/tileSize; i < (minX-(minX%tileSize)+posXTiles*tileSize)/tileSize; i++) {
			for(int u = (minY-(minY%tileSize))/tileSize; u < (minY-(minY%tileSize)+posYTiles*tileSize)/tileSize; u++) {
				// If the tile is taken, it should always be pushed into the vector, else we have to check if the user only wants the taken tiles.
				if(tiles[i][u].GetTaken() || !OnlyReturnTaken) { 
					collisionArray.push_back(&tiles[i][u]);
				}
			}
		}
		
		return collisionArray;
	}

	
	void UnSetTilesTaken(vector<cdTile *> vec, string sOccupier, int id) {
		for(unsigned int i = 0; i<vec.size(); i++) {
			vec[i]->unset(sOccupier, id);
		}
	}

	void UnSetTilesTaken(int minX, int minY, int maxX, int maxY, string sOccupier) {
		int posXTiles;
		if((maxX-minX)%tileSize==0) {posXTiles = (maxX-minX)/tileSize;}
		else {posXTiles = ((maxX-minX)+tileSize)/tileSize;}

			
		int posYTiles;
		if((maxY-minY)%tileSize==0) {posYTiles = (maxY-minY)/tileSize;}
		else {posYTiles = ((maxY-minY)+tileSize)/tileSize;}

		for(int i = (minX-(minX%tileSize))/tileSize; i < (minX-(minX%tileSize)+posXTiles*tileSize)/tileSize; i++) {
			for(int u = (minY-(minY%tileSize))/tileSize; u < (minY-(minY%tileSize)+posYTiles*tileSize)/tileSize; u++) {
				tiles[i][u].unset(sOccupier, -1);
			}
		}
	}


	////////////////////////////////////////////////////////////////
	// ---------------- Set Taken Tiles --------------------------//
	////////////////////////////////////////////////////////////////


	void SetTilesTaken(int minX, int minY, int maxX, int maxY, string sOccupier) {
		int posXTiles;
		if((maxX-minX)%tileSize==0) {posXTiles = (maxX-minX)/tileSize;}
		else {posXTiles = ((maxX-minX)+tileSize)/tileSize;}

		
		int posYTiles;
		if((maxY-minY)%tileSize==0) {posYTiles = (maxY-minY)/tileSize;}
		else {posYTiles = ((maxY-minY)+tileSize)/tileSize;}

		for(int i = (minX-(minX%tileSize))/tileSize; i < (minX-(minX%tileSize)+posXTiles*tileSize)/tileSize; i++) {
			for(int u = (minY-(minY%tileSize))/tileSize; u < (minY-(minY%tileSize)+posYTiles*tileSize)/tileSize; u++) {
				tiles[i][u].setTaken(new string(sOccupier), -1);
			}
		}
	}

	vector<cdTile *> SetTilesTaken(int minX, int minY, int maxX, int maxY, string sOccupier, int id) {
		int posXTiles;
		if((maxX-minX)%tileSize==0) {posXTiles = (maxX-minX)/tileSize;}
		else {posXTiles = ((maxX-minX)+tileSize)/tileSize;}

		
		int posYTiles;
		if((maxY-minY)%tileSize==0) {posYTiles = (maxY-minY)/tileSize;}
		else {posYTiles = ((maxY-minY)+tileSize)/tileSize;}

		vector<cdTile *> yourTiles;	

		for(int i = (minX-(minX%tileSize))/tileSize; i < (minX-(minX%tileSize)+posXTiles*tileSize)/tileSize; i++) {
			for(int u = (minY-(minY%tileSize))/tileSize; u < (minY-(minY%tileSize)+posYTiles*tileSize)/tileSize; u++) {
				yourTiles.push_back(&tiles[i][u]);
				tiles[i][u].setTaken(new string(sOccupier), id);
			}
		}

		return yourTiles;
	}
}