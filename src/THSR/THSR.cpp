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


    for(vector<string>::iterator page_it = pages.begin() ;
        page_it != pages.end() ; ++page_it)
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
        string mHttpVer, mHttpData ;
        u_int mHttpCode ;

        stream >> mHttpVer >> mHttpCode ;

        if (!stream || mHttpVer.substr(0,5) != "HTTP/" || mHttpCode != 200)
            return STATE_NETWORK_ERROR ;
        else
            qDebug() << mHttpVer.c_str() << " " << mHttpCode << endl;


        std::stringstream web_stream ;
        web_stream << stream.rdbuf() ;
        while (std::getline(web_stream, mHttpData))
        {
            qDebug() << mHttpData.c_str() ;
        }

        stream.close();
    }


    /*

    std::stringstream ostream ;
    ostream << stream.rdbuf() ;

    string html = ostream.str() ;
    qDebug() << html.c_str() << endl ;
    */
    return STATE_SUCCESS ;
}

}
