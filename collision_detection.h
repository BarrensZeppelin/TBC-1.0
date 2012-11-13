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
using namespace std;

namespace tbc {
	class cdTile {
	private:
		int tX;
		int tY;

		vector<string> occupiers;
		vector<int> occupierIDs;

	public:
		cdTile();
		~cdTile(){}
		
		void setTaken(string sTakenBy, int id) {
			occupiers.push_back(sTakenBy);
			occupierIDs.push_back(id);
		}
		
		void unset(string sTakenBy, int id) {
			if(id == -1) {
				int index = -1;
				for(unsigned int i = 0; i < occupiers.size() && index==-1; i++) {
					if(occupiers[i].compare(sTakenBy)==0) {
						index = i;
						occupiers.erase(occupiers.begin()+index);
					}
				}
				occupierIDs.erase(occupierIDs.begin()+index);
			} else {
				int index = -1;
				for(unsigned int k = 0; k<occupiers.size() && index==-1; k++) {
					if(occupierIDs[k]==id) {
						if(occupiers[k].compare(sTakenBy)==0) { 
							index = k;
							occupiers.erase(occupiers.begin()+index);
						}
					}
				}
				occupierIDs.erase(occupierIDs.begin()+index);
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
			bool present = false;
			for(unsigned int i = 0; i<occupiers.size() && present == false; i++) {
				if(occupiers[i].compare(check)==0) {
					present = true;
				}	
			}
			return present;
		}
	};

	vector<vector<cdTile> > tiles;
	//vector<cdTile *> takenTiles;

	int tileSize = 8; //Size of tiles defaults to 8, this can be changed at any time.


	cdTile::cdTile() {
		tX = 0;
		tY = 0;
	}



	void initCollisionTiles(int areaWidth, int areaHeight) {
		tiles.clear();

		int xTiles = areaWidth/tileSize;
		int yTiles = areaHeight/tileSize;

		tiles.resize(xTiles);
		for(int i=0; i<xTiles; i++) {
			tiles[i].resize(yTiles);
		}
		
		for(int i=0; i < xTiles; i++) {
			for(int u=0; u < yTiles; u++) {
				tiles[i][u].setValues(i*tileSize, u*tileSize);
			}
		}
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

		if(!OnlyReturnTaken) {
			for(int i = (minX-(minX%tileSize))/tileSize; i < (minX-(minX%tileSize)+posXTiles*tileSize)/tileSize; i++) {
				for(int u = (minY-(minY%tileSize))/tileSize; u < (minY-(minY%tileSize)+posYTiles*tileSize)/tileSize; u++) {
					collisionArray.push_back(&tiles[i][u]);
				}
			}
		} else {
			for(int i = (minX-(minX%tileSize))/tileSize; i < (minX-(minX%tileSize)+posXTiles*tileSize)/tileSize; i++) {
				for(int u = (minY-(minY%tileSize))/tileSize; u < (minY-(minY%tileSize)+posYTiles*tileSize)/tileSize; u++) {
					if(tiles[i][u].GetTaken()) {
						collisionArray.push_back(&tiles[i][u]);
					}
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
				tiles[i][u].setTaken(sOccupier, -1);
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
				tiles[i][u].setTaken(sOccupier, -1);
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
				tiles[i][u].setTaken(sOccupier, id);
			}
		}

		return yourTiles;
	}
}