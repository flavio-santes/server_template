/*
 * Copyright (C) 2017 1byt3, customers at 1byt3.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ClientWorker.hpp"

#include <iostream>
#include <QThread>

ClientWorker::ClientWorker(int socketDescriptor, QObject *parent) : QObject(parent)
{
	this->socketDescriptor = socketDescriptor;
}

void ClientWorker::readyRead()
{
	QByteArray data = socket->readAll();

	std::cout << "Data available: " << data.size() << "\n" << std::flush;
}

void ClientWorker::finished()
{
	socket->disconnectFromHost();
	socket->deleteLater();
}

void ClientWorker::started()
{
	std::cout << __func__ << ", thread: "
		  << QThread::currentThreadId() << "\n";

	socket = new QTcpSocket(this);
	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

	if (socket->setSocketDescriptor(socketDescriptor) == false)
	{
		std::cout <<  __func__ << ": Error\n" << std::flush;

		emit error(1);
	}
}

