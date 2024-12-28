/**
 * @file FakeAPlib.cpp
 * @author NGUEYOU SIMO, Neil L. (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-12-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "FakeAPlib.h"

bool FakeAp::initialize(const String &ssid, const String &psw)
{
	if (!m_sdManager->initialize())
	{
		Serial.println(ERROR_SD_NOT_INIT);
		return 0;
	}

	Serial.println("Configurando el punto de acceso.....");

	// se crea un punto de acceso con el nombre declarado más arriba
	if (psw == NULL)
	{
		if (!WiFi.softAP(ssid)) // está función accepta un segundo parametro opcional que será la contraseña del wifi
		{
			log_e(ERROR_WIFIAP_CREATE);
			return 0;
		}
	}
	else
	{
		if (!WiFi.softAP(ssid, psw))
		{
			log_e(ERROR_WIFIAP_CREATE);
			return 0;
		}
	}

	// imprimir por pantalla la dirección IP de la red
	Serial.println(SUCCESS_WIFIAP_INIT);
	Serial.print("Access point IP address: ");
	Serial.println(WiFi.softAPIP());
	this->start();
	m_sdManager->logEvent(String(millis()) + ": Server started");
	return 1;
}

bool FakeAp::setWifi(const String &wifissid, const String &wifipsw)
{
	Serial.printf("Connectando a %s", wifissid.c_str());
	// Serial.print(wifissid);
	wifipsw != NULL ? WiFi.begin(wifissid, wifipsw) : WiFi.begin(wifissid);

	/*if (wifipsw != NULL)
	{
	  WiFi.begin(wifissid, wifipsw);
	}
	else
	{
	  WiFi.begin(wifissid);
	}*/

	unsigned long timeout = millis();
	while (WiFi.status() != WL_CONNECTED)
	{
		if ((millis() - timeout) > TIME_OUT_LIMIT)
			return 0;
		delay(500);
		Serial.print(".");
	}

	Serial.println();
	Serial.printf(SUCCESS_WIFISTA_CONECT, wifissid.c_str());
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	m_sdManager->logEvent(String(millis()) + ": connected to network " + wifissid);
	m_sdManager->logEvent("IP address: " + WiFi.localIP());

	return 1;
}

void FakeAp::checkState()
{
	if (!m_sdManager->isCardInit())
	{
		if(WITH_ERROR_TYPE) Serial.println(ERROR_SD_CARD_READ);
		return;
	}
	
	Serial.println("Files path: ");
	Serial.println("Authentication page file: " +
					m_authPage + ": "+ m_sdManager->getFileSystem().exists(m_authPage));
	Serial.println("Thanks page file: " +
					m_thksPage + ": "+ m_sdManager->getFileSystem().exists(m_thksPage));
	Serial.println("Admin page file: " +
					m_adminPage + ": "+ m_sdManager->getFileSystem().exists(m_adminPage));
	Serial.println("Data file: " +
					m_datafile + ": "+ m_sdManager->getFileSystem().exists(m_datafile));
	Serial.println("Log file: " +
					String(LOG_FILE) + ": " + m_sdManager->getFileSystem().exists(LOG_FILE));

}
