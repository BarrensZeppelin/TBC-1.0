/*
	*	Tile Based Collision Detection
	*
	*	Written and edited by:
	*		Oskar Haarklou Veileborg
	*	2012	ohv1020@hotmail.com
	*
*/

#pragma once

#ifndef TBC
#define TBC

#include <math.h>
#include <vector>
#include <string>
#include <time.h>
#include <iostream>
using namespace std;

namespace tbc {
	class cdTile {
	private:
		int tX;
		int tY;

		vector<string *> occupiers;
		vector<int> occupierIDs;

	public:
		cdTile(int x, int y);
		cdTile();
		~cdTile();

		void SetTaken(string * sTakenBy, int id);
		
		void Unset(string, int);

		// ------------ Getters ----------------
		int GetX();
		int GetY();

		bool GetTaken();
		bool GetTaken(string check);
		
		pair<vector<string *> *, vector<int> *> GetInfo();
	};

	extern vector<vector<cdTile> > tiles;

	extern int tileSize; //Size of tiles defaults to 8, this can be changed at any time.
	
	extern vector<cdTile *> lastCollisions;
	
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
	vector<cdTile *> GetLastCollisions();


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
	void InitCollisionTiles(unsigned int areaWidth, unsigned int areaHeight, bool verbDebug = false);


	
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
	bool CheckCollision(int minX, int minY, int maxX, int maxY, string sOccupier = "", bool stopAfterFirst = false);

	
	///////////////////////////////////////////////////////////////
	/// \brief BETA Checks for collisions in a line 
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
	bool LineCheck(int x0, int y0, int x1, int y1, int xOffset, int yOffset, int objWidth, int objHeight, string sOccupier = "");


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
	vector<cdTile *> CollisionArray(int minX, int minY, int maxX, int maxY, bool OnlyReturnTaken = true);


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
	void UnSetTilesTaken(vector<cdTile *> vec, string sOccupier, int id);
	

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
	void UnSetTilesTaken(int minX, int minY, int maxX, int maxY, string sOccupier, int id = -1);


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
	vector<cdTile *> SetTilesTaken(int minX, int minY, int maxX, int maxY, string sOccupier, int id = -1);
}


#endif