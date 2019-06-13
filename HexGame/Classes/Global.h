#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H_


#define UPDATESCOR_EVENT "update_score_event"

#define gMapLineM 9
#define gMapRowM 9

#define gSpriteW 78
#define gSpriteH 90

#define gSpace 3.5


#define gScoreBase 40
#define gScoreLabelSpace 20
#define gScoreLabelFontSize 40
#define gHighestLSpaceY 30
#define gHighestLSpaceX 200
#define gHighestLFontSize 30
#define gDoWellLFontSize 40

#define MAP_EMPTY 0
#define MAP_FILL 1
#define MAP_NOUSE 2
#include <string>
#include <vector>
using std::string;
using std::vector;
class Global
{
public:
	Global();
	~Global();
	static string gameSessionId;
	

	static string toString(vector<char> *buffer);

	static string getSessionIdFromHeader(string header);
	static int maxscore;
	static string user;
};

#endif