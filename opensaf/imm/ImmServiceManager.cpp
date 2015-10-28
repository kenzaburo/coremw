/*
 * ImmServiceManager.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: trunghuynh
 */

#include "ImmServiceManager.h"
#include <iostream>
#include <sqlite3.h>
#include <stdio.h>
#include <string>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

bool ImmServiceManager::readDataReport() {
	std::cout << "on readDataReport";

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	string sql;
	const char* data = "Callback function called";

	/* Open database */
	rc = sqlite3_open(this->dbName.c_str(), &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		//		exit(0);
		return false;
	} else {
		fprintf(stderr, "Opened database successfully\n");
	}

	/* Create SQL statement */
	sql = "SELECT * from " + tbName;

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql.c_str(), callback, (void*) data, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}
	sqlite3_close(db);

	return true;
}

bool ImmServiceManager::writeDataReport(string cpuLoad, string ramLoad) {
	std::cout << "on writeDataReport()";
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	string sql;

	/* Open database */
	rc = sqlite3_open(this->dbName.c_str(), &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		//		exit(0);
		return false;
	} else {
		fprintf(stderr, "Opened database successfully\n");
	}

	sql = "insert into " + tbName + "(CPU_LOAD,RAM_LOAD) VALUES (" + cpuLoad + "," + ramLoad + ")";
	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Records created successfully\n");
	}

	sqlite3_close(db);
	return true;
}

void ImmServiceManager::setup() {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	string sql;
	const char* dbname = this->dbName.c_str();
	fprintf(stderr, "database name: %s\n table:%s\n",this->dbName.c_str(),this->tbName.c_str());
	/* Open/create an database */
	rc = sqlite3_open(dbname, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return;
	} else {
		fprintf(stderr, "Opened database successfully\n");
	}
	/* Create SQL statement */
	sql = "CREATE TABLE " + this->tbName + "("
			"ID  INTEGER PRIMARY KEY   AUTOINCREMENT,"
			"CPU_LOAD           TEXT    NOT NULL,"
			"RAM_LOAD            INT     NOT NULL);";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Table created successfully\n");
	}
	sqlite3_close(db);
}

ImmServiceManager::ImmServiceManager() {
	// TODO Auto-generated constructor stub
	// create an database if there is no
	dbName = "/home/trunghuynh/c_plus_plus/coremw/db/system_load.db";
	tbName = "system_load_tb";
}

ImmServiceManager::~ImmServiceManager() {
	// TODO Auto-generated destructor stub
}

