#pragma once

struct Attraction
{
	string attractionName;
	string attractionDesc;
	int no;
	Attraction() {
		attractionName = "";
		attractionDesc = "";
		no = -1;
	}
	Attraction(char * attractionName, char * attractionDesc, int no) {
		this->attractionName = attractionName;
		this->attractionDesc = attractionDesc;
		this->no = no;
	}
};