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
#include "Server.hpp"

#include <QHostAddress>
#include <iostream>
#include "QThread"

Server::Server(QObject *parent) : QTcpServer(parent)
{
	this->listen(QHostAddress("127.0.0.1"), 9999);
}

Server::~Server()
{
	auto it = allThreads.begin();

	while (it != allThreads.end()) {
		QThread *thread = *it;

		std::cout << "Stopping thread: "
			  << thread->currentThreadId()
			  << "\n" << std::flush;

		thread->quit();
		if (thread->wait(1000) == false) {
			std::cout << "Error terminating thread: "
				  << thread->currentThreadId()
				  << "\n" << std::flush;
		}

		allThreads.erase(it);
		it++;
	}
}

void Server::incomingConnection(qintptr socketDescriptor)
{
	std::cout << __func__ << ", thread: "
		  << QThread::currentThreadId() << "\n";

	ClientWorker *worker = new ClientWorker(socketDescriptor);
	QThread *thread = new QThread(this);

	connect(thread, SIGNAL(finished()), worker, SLOT(finished()));
	connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

	connect(thread, SIGNAL(started()), worker, SLOT(started()));

	worker->moveToThread(thread);
	thread->start();

	allThreads.push_back(thread);
}

