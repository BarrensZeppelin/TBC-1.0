#include "collision_detection.hpp"

namespace tbc {
	cdTile::cdTile(int x, int y) : tX(x), tY(y) {}
	cdTile::cdTile() : tX(0), tY(0) {}
	cdTile::~cdTile() {
		for(unsigned int i=0; i<occupiers.size(); i++) {
			delete occupiers[i];
		}
	}

	void cdTile::SetTaken(string * sTakenBy, int id) {
		occupiers.push_back(sTakenBy);
		occupierIDs.push_back(id);
	}

	void cdTile::Unset(string sTakenBy, int id) {
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

	int cdTile::GetX() {return tX;}
	int cdTile::GetY() {return tY;}

	bool cdTile::GetTaken() {return !occupiers.empty();}
	bool cdTile::GetTaken(string check) {
		for(unsigned int i = 0; i<occupiers.size(); i++) {
			if(occupiers[i]->compare(check)==0) {
				return true;
			}	
		}
		return false;
	}

	pair<vector<string *> *, vector<int> *> cdTile::GetInfo() {
		pair<vector<string *> *, vector<int> *> p(&occupiers, &occupierIDs);
		return p;
	}
	//// ENDOF cdTile ///////////////////////////////


	vector<vector<cdTile>> tiles = vector<vector<cdTile>>();

	int tileSize = int(8);

	vector<cdTile *> lastCollisions = vector<cdTile *>();


	vector<cdTile *> GetLastCollisions() {return lastCollisions;}



	void InitCollisionTiles(unsigned int areaWidth, unsigned int areaHeight, bool verbDebug) {
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
		
		if(verbDebug) {
			cout << "Creation of Collision Tile Array took " << (unsigned long) (clock() - begTime) / CLOCKS_PER_SEC << "seconds"<< endl;
		}
	}



	bool CheckCollision(int minX, int minY, int maxX, int maxY, string sOccupier, bool stopAfterFirst) {

		bool error[8] = {false, false, false, false, false, false, false, false};
		if(minX < 0) {minX = 0;error[0]=true;} if(minX > (tiles.size()-1)*tileSize) {minX = (tiles.size()-1)*tileSize;error[1]=true;}
		if(maxX < 0) {maxX = 0;error[2]=true;} if(maxX > (tiles.size()-1)*tileSize) {maxX = (tiles.size()-1)*tileSize;error[3]=true;}

		if(minY < 0) {minY = 0;error[4]=true;} if(minY > (tiles[0].size()-1)*tileSize) {minY = (tiles[0].size()-1)*tileSize;error[5]=true;}
		if(maxY < 0) {maxY = 0;error[6]=true;} if(maxY > (tiles[0].size()-1)*tileSize) {maxY = (tiles[0].size()-1)*tileSize;error[7]=true;}

		#ifdef _DEBUG
		
		//if(error[0]) {cerr << 

		#endif

		
		lastCollisions.clear();
		bool useIdent = true;
		if(sOccupier.compare("") == 0) {useIdent = false;}
		
		int posXTiles;
		if((maxX-minX)%tileSize==0) {posXTiles = (maxX-minX)/tileSize;}
		else {posXTiles = ((maxX-minX)+tileSize)/tileSize;}

		
		int posYTiles;
		if((maxY-minY)%tileSize==0) {posYTiles = (maxY-minY)/tileSize;}
		else {posYTiles = ((maxY-minY)+tileSize)/tileSize;}
		
		bool collision = false;
		
		for(int i = (minX-(minX%tileSize))/tileSize; i < (minX-(minX%tileSize)+posXTiles*tileSize)/tileSize && (!collision || !stopAfterFirst); i++) {
			for(int u = (minY-(minY%tileSize))/tileSize; u < (minY-(minY%tileSize)+posYTiles*tileSize)/tileSize; u++) {
				if(useIdent) {
					if(tiles[i][u].GetTaken(sOccupier)) {
						collision = true;
						lastCollisions.push_back(&tiles[i][u]);
						if(stopAfterFirst) {break;}
					}
				} else {
					if(tiles[i][u].GetTaken()) {
						collision = true;
						lastCollisions.push_back(&tiles[i][u]);
						if(stopAfterFirst) {break;}
					}
				}
			}
		}
		
		return collision;
	}


	
	bool LineCheck(int x0, int y0, int x1, int y1, int xOffset, int yOffset, int objWidth, int objHeight, string sOccupier) {
		bool useIdent = true;
		if(sOccupier.compare("") == 0) {useIdent = false;}


		// Two quick checks at the start and endpoint
		if(useIdent) {
			if(CheckCollision(x0 + xOffset, y0 + yOffset, x0 + objWidth + xOffset, y0 + objHeight + yOffset, sOccupier)) {return true;}
			if(CheckCollision(x1 + xOffset, y1 + yOffset, x1 + objWidth + xOffset, y1 + objHeight + yOffset, sOccupier)) {return true;}
		} else {
			if(CheckCollision(x0 + xOffset, y0 + yOffset, x0 + objWidth + xOffset, y0 + objHeight + yOffset)) {return true;}
			if(CheckCollision(x1 + xOffset, y1 + yOffset, x1 + objWidth + xOffset, y1 + objHeight + yOffset)) {return true;}
		}
		
		
		x0 = floor((float)x0 / objWidth);
		y0 = floor((float)y0 / objHeight);
		x1 = floor((float)x1 / objWidth);
		y1 = floor((float)y1 / objHeight);

		///////////////////////////////////////////////////////////////////
		bool steep = abs(y1-y0) > abs(x1-x0);
		
		if(steep) {
			swap(x0, y0);
			swap(x1, y1);
		}
		if(x0 > x1) {
			swap(x0, x1);
			swap(y0, y1);
		}
		
		int deltax = x1 - x0;
		int deltay = abs(y1 - y0);

		double error = 0;
		double deltaerr;
		if(deltax!=0) {deltaerr = double(deltay) / double(deltax);}
		else deltaerr = 0;

		int ystep;
		int y = y0;
		if (y0 < y1) {ystep=1;} else {ystep=-1;}
		
		bool collision = false;

		for(int x=x0; x<=x1 && !collision; x++) {
			int rx = x;
			int ry = y;
			if(steep) {rx = y; ry = x;}

			if(useIdent) {
				collision = CheckCollision(rx*objWidth + xOffset, ry*objHeight + yOffset, (rx+1)*objWidth + xOffset, (ry+1)*objHeight + yOffset, sOccupier);
			} else {
				collision = CheckCollision(rx*objWidth + xOffset, ry*objHeight + yOffset, (rx+1)*objWidth + xOffset, (ry+1)*objHeight + yOffset);
			}

			error+=deltaerr;
			if(error>0.5) {
				y += ystep;
				error -= 1.0;
			}
		}
		/////////////////////////////////////////////////////////////////////////////////////
		
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
			vec[i]->Unset(sOccupier, id);
		}
	}



	void UnSetTilesTaken(int minX, int minY, int maxX, int maxY, string sOccupier, int id) {
		//std::cout << minY << " " << maxY << std::endl;
		int posXTiles;
		if((maxX-minX)%tileSize==0) {posXTiles = (maxX-minX)/tileSize;}
		else {posXTiles = ((maxX-minX)+tileSize)/tileSize;}

			
		int posYTiles;
		if((maxY-minY)%tileSize==0) {posYTiles = (maxY-minY)/tileSize;}
		else {posYTiles = ((maxY-minY)+tileSize)/tileSize;}

		for(int i = (minX-(minX%tileSize))/tileSize; i < (minX-(minX%tileSize)+posXTiles*tileSize)/tileSize; i++) {
			for(int u = (minY-(minY%tileSize))/tileSize; u < (minY-(minY%tileSize)+posYTiles*tileSize)/tileSize; u++) {
				tiles[i][u].Unset(sOccupier, id);
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
				if(id!=0) {yourTiles.push_back(&tiles[i][u]);}
				tiles[i][u].SetTaken(new string(sOccupier), id);
			}
		}
		
		return yourTiles;
	}
}