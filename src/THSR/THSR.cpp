/**
    Provide by Walon Li

    File: THSR.h
**/

#include "THSR.h"
#include <QDebug>
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
        if (connect_server(30) != STATE_SUCCESS)
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

        stream >> mHttpVer >> mHttpCode ;

        if (!stream || mHttpVer.substr(0,5) != "HTTP/" || mHttpCode != 200)
            return STATE_NETWORK_ERROR ;
        else
            qDebug() << mHttpVer.c_str() << " " << mHttpCode << endl;


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
                    if (data.at(23) == '-')
                        time = new QTime(0,0) ;
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
    }

    return STATE_SUCCESS ;
}

}
