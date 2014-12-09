/**
    Provide by Walon Li

    File: THSR.cpp
**/

#include "THSR.h"
#include "boost/format.hpp"
#include <QDebug>
#include <QtTest>
#include <iostream>

namespace ttp {

STATE THSR::parse_data_from_web()
{
    vector<string> pages = get_web_pages() ;

    const string match_pattern("<tr bgcolor=\"") ;
    const string data_pre_pattern("<td title=\"");
    const string data_mid_pattern("\">") ;
    const string data_pos_pattern("</td>") ;
    const string data_end_pattern("<td title=\"\"><\/td>>") ;


    for(auto page_it = pages.begin() ; page_it != pages.end() ; ++page_it)
    {
        // Try to connect page.
        if (connect_server(10) != STATE_SUCCESS)
            return STATE_NETWORK_ERROR ;
        boost::asio::ip::tcp::iostream & stream = get_stream() ;

        // fill http header
        stream << "GET " << *page_it << " HTTP/1.1\r\n" ;
        stream << "Host: " << get_web_server() << "\r\n" ;
        stream << "Accept: */*\r\n" ;
        stream << "Cache-Control: no-cache\r\n" ;
        stream << "Connection: close\r\n\r\n" ;
        stream.flush() ;



        // Recieve header and check is invalid or not.
        string mHttpVer, data ;
        u_int mHttpCode ;

        qDebug() << "Get"<< get_web_server().c_str() << (*page_it).c_str() <<"..." ;
        stream >> mHttpVer >> mHttpCode ;
        if (!stream || mHttpVer.substr(0,5) != "HTTP/" || mHttpCode != 200)
            return STATE_NETWORK_ERROR ;
        else
            qDebug() << mHttpVer.c_str() << " " << mHttpCode << "OK" << endl;


        std::stringstream web_stream ;
        web_stream << stream.rdbuf() ;
        while (std::getline(web_stream, data))
        {
            // clean white space
            data = ttp::trim(data) ;
            if (data.empty() || data.size() < match_pattern.size())
                continue ;


            // match pattern "<tr bgcolor"
            if (!data.substr(0, match_pattern.size()).compare(match_pattern))
            {
                std::getline(web_stream, data) ;
                data = ttp::trim(data) ;

                // generate Train object.
                Train train(data.substr(4, 4), "") ;
                while(std::getline(web_stream, data))
                {
                    data = ttp::trim(data) ;
                    if (data.empty() || data.size() < data_end_pattern.size()) break;

                    string station(data.substr(11, 9)) ;
                    QTime *time ;
                    if (data.at(22) == '-')
                        time = new QTime() ;
                    else
                        time = new QTime(atoi(data.substr(22,2).c_str()),
                                         atoi(data.substr(25,2).c_str())) ;

                    // add data to train
                    pair<string, QTime> p(station, *time) ;
                    train.add_time_into_schedule(p) ;

                    // help to generate station map.
                    this->add_station_into_map(station) ;
                    delete time ;
                }

                this->add_train_into_table(train) ;
            }
        }
        stream.close();
        QTest::qWait(100) ;
    }

    return STATE_SUCCESS ;
}

STATE THSR::get_list_with_user_input(QDate date, string start, string arrival, QStringList & list)
{
    vector<Train> table = get_table() ;


    for (auto it = table.begin() ; it != table.end() ; ++it)
    {
        QTime *start_time = 0, *arrival_time = 0 ;
        vector< pair<string, QTime> > schedule = (*it).get_schedule() ;
        for (auto it2 = schedule.begin() ; it2 != schedule.end() ; ++it2)
        {
            // check time is legal.
            if ((*it2).second.isNull()) continue ;

            if (!(*it2).first.compare(start))
                start_time = new QTime((*it2).second) ;
            else if (!(*it2).first.compare(arrival) && start_time)
                arrival_time = new QTime((*it2).second) ;

            if (start_time && arrival_time) break ;
        }


        if (start_time && arrival_time)
        {
            string str = (boost::format("車號:%s\t    出發:%-10s抵達:%-10s%s") \
                                        % it->get_id().c_str() \
                                        % start_time->toString("hh:mm").toStdString().c_str() \
                                        % arrival_time->toString("hh:mm").toStdString().c_str()\
                                        % it->get_help().c_str()).str() ;
            list.push_back(str.c_str());
        }

        delete start_time ;
        delete arrival_time ;
    }
    return STATE_SUCCESS ;
}
}
