/*
 * ImmServiceManager.h
 *
 *  Created on: Oct 7, 2015
 *      Author: trunghuynh
 */

#ifndef IMMSERVICEMANAGER_H_
#define IMMSERVICEMANAGER_H_

class ImmServiceManager {
public:
	ImmServiceManager();
	virtual ~ImmServiceManager();
	bool saveDataReport();
	bool readDataReport();
};

#endif /* IMMSERVICEMANAGER_H_ */
