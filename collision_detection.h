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
			return false;
		}
		
		pair<vector<string *> *, vector<int> *> GetInfo() {
			pair<vector<string *> *, vector<int> *> p(&occupiers, &occupierIDs);
			return p;
		}
	};

	vector<vector<cdTile> > tiles;

	int tileSize = 8; //Size of tiles defaults to 8, this can be changed at any time.
	
	vector<cdTile *> lastCollisions;
	
	///////////////////////////////////////////////////////////////
	/// \brief Return a vector of the last found collisions
	///
	/// After a call to CheckCollision, you can retrieve the
	/// the pointers to the tiles that returned true during
	/// the check by calling this function.
	///
	/// \see CheckCollision
	///
	///////////////////////////////////////////////////////////////
	vector<cdTile *> GetLastCollisions()	{return lastCollisions;}


	///////////////////////////////////////////////////////////////
	/// \brief Initialise the Collision Detection System for use
	/// 
	/// This is required before using any of the functions included
	/// in this collision detection library.
	/// You can also use this function to reset the current
	/// states of tiles.
	///
	/// \param areaWidth	Width of the area you want to initialise.
	/// \param areaHeight	Height of the area you want to initialise.
	/// \param verbDebug	Include message in prompt with loadtime?
	///
	///////////////////////////////////////////////////////////////
	void InitCollisionTiles(unsigned int areaWidth, unsigned int areaHeight, bool verbDebug = false) {
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


	
	///////////////////////////////////////////////////////////////
	/// \brief Check a rectangle for collisions
	///
	/// This is the main function of TBC.
	/// Use it to check for a collision against tiles
	/// claimed by a specific identifier, or just if any
	/// tiles within the specified rectangle are taken.
	///
	/// Do feel free to create your own wrapper
	/// functions for simplified use.
	///
	/// \param minX 	The left coordinate of your rectangle
	/// \param minY 	The top coordinate of your rectangle
	/// \param maxX 	The right coordinate of your rectangle
	/// \param maxY 	The bottom coordinate of your rectangle
	/// \param sOccupier The identifier to check against (if specified)
	/// \param stopAfterFirst Should the function stop checking for collision once one has been found?
	///
	/// \see GetLastCollisions
	///
	///////////////////////////////////////////////////////////////
	bool CheckCollision(int minX, int minY, int maxX, int maxY, string sOccupier = "", bool stopAfterFirst = false) {
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
	
	///////////////////////////////////////////////////////////////
	/// \brief Checks for collisions in a line
	///
	/// This can be especially useful for objects moving at very
	/// fast speeds, to check if an object has passed through
	/// another, rather than checking if the object has been on
	/// top of another object.
	///
	/// \param x0 Line-Coordinate x0 (Start point)
	/// \param y0 Line-Coordinate y0 (Start point)
	/// \param x1 Line-Coordinate x1 (Endpoint)
	/// \param y1 Line-Coordinate y1 (Endpoint)
	/// \param xOffset Offset amount on the x-axis. Used for collision checking
	/// \param yOffset Offset amount on the y-axis. Used for collision checking
	/// \param objWidth Width of the object you are checking for
	/// \param objHeight Height of the object you are checking for
	/// \param sOccupier The identifier to check against (if specified)
	///
	/// \see CheckCollision, GetLastCollisions
	///
	///////////////////////////////////////////////////////////////
	bool LineCheck(int x0, int y0, int x1, int y1, int xOffset, int yOffset, int objWidth, int objHeight, string sOccupier = "") {
		bool useIdent = true;
		if(sOccupier.compare("") == 0) {useIdent = false;}
		
		x0 = floor(x0 / objWidth);
		y0 = floor(y0 / objHeight);
		x1 = floor(x1 / objWidth);
		y1 = floor(y1 / objHeight);

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

		for(int x=x0; x<=x && !collision; x++) {
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

	///////////////////////////////////////////////////////////////
	/// \brief Returns a vector with pointers to the tiles in the given area
	///
	/// Use this when you need to access every tile inside an area.
	/// Maybe you want to check if an area has tiles with multiple
	/// identifiers?
	///
	/// \param minX 	The left coordinate of your rectangle
	/// \param minY 	The top coordinate of your rectangle
	/// \param maxX 	The right coordinate of your rectangle
	/// \param maxY 	The bottom coordinate of your rectangle
	/// \param OnlyReturnTaken Should the vector only contain taken tiles?
	///
	/// \see CheckCollision
	///
	///////////////////////////////////////////////////////////////
	vector<cdTile *> CollisionArray(int minX, int minY, int maxX, int maxY, bool OnlyReturnTaken = true) {
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

	///////////////////////////////////////////////////////////////
	/// \brief Dismisses ownership of tiles
	///
	/// This function removes all traces of occupiers
	/// that match the given sOccupier and the given id.
	/// The tiles in this vector will still retain
	/// all other claims of ownership with other occupiers
	/// and IDs.
	///
	/// \param vec Vector returned from SetTilesTaken
	/// \param sOccupier The identifier to check against.
	/// \param id The ID to check against.
	///
	/// \see SetTilesTaken
	///
	///////////////////////////////////////////////////////////////
	void UnSetTilesTaken(vector<cdTile *> vec, string sOccupier, int id) {
		for(unsigned int i = 0; i<vec.size(); i++) {
			vec[i]->unset(sOccupier, id);
		}
	}
	
	///////////////////////////////////////////////////////////////
	/// \brief Removes all instances of the given occupier from the tiles in the given area
	///
	/// \param minX 	The left coordinate of your rectangle
	/// \param minY 	The top coordinate of your rectangle
	/// \param maxX 	The right coordinate of your rectangle
	/// \param maxY 	The bottom coordinate of your rectangle
	/// \param sOccupier The identifier to check against.
	///
	/// \see SetTilesTaken, InitCollisionTiles
	///
	///////////////////////////////////////////////////////////////
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

	///////////////////////////////////////////////////////////////
	/// \brief Set ownership of tiles in the given area
	///
	/// Use it to set the states of the tiles in the area
	/// to also include sOccupier and id (if specified).
	///
	/// Do feel free to create your own wrapper
	/// functions for simplified use.
	///
	/// \param minX 	The left coordinate of your rectangle
	/// \param minY 	The top coordinate of your rectangle
	/// \param maxX 	The right coordinate of your rectangle
	/// \param maxY 	The bottom coordinate of your rectangle
	/// \param sOccupier The identifier to fill into the tiles
	/// \param id		The id to fill into the tiles (optional)
	///
	/// \see UnSetTilesTaken
	///
	///////////////////////////////////////////////////////////////
	vector<cdTile *> SetTilesTaken(int minX, int minY, int maxX, int maxY, string sOccupier, int id = 0) {
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
				tiles[i][u].setTaken(new string(sOccupier), id);
			}
		}
		
		return yourTiles;
	}
}