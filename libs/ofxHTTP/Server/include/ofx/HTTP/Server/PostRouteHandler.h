// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include <istream>
#include "Poco/Buffer.h"
#include "Poco/Exception.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/NameValueCollection.h"
#include "ofLog.h"
#include "ofx/HTTP/Types/AbstractTypes.h"
#include "ofx/HTTP/Server/PostRouteEvents.h"
#include "ofx/HTTP/Server/PostRouteFileHandler.h"
#include "ofx/HTTP/Server/PostRouteSettings.h"


namespace ofx {
namespace HTTP {


class PostRoute;


class PostRouteHandler: public AbstractRouteHandler
    /// \brief A flexible POST route handler.
    /// \details Form data must be encoded with "multipart/form-data" or
    ///         "application/x-www-form-urlencoded".  "text/plain"
    ///         form encoding is supported, but not parsed.
{
public:
    typedef PostRouteSettings Settings;
        ///< \brief A typedef for PostRouteSettings

    PostRouteHandler(PostRoute& parent);
        ///< \brief Create a PostRouteHandler.
        ///< \param parent The parent PostRoute.

    virtual ~PostRouteHandler();
        ///< \brief Destroy the PostRouteHandler.
    
    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response);

    static const Poco::Net::MediaType POST_CONTENT_TYPE_MULTIPART;
        ///< \brief A constant defining "multipart/form-data".

    static const Poco::Net::MediaType POST_CONTENT_TYPE_URLENCODED;
        ///< \brief A constant defining "application/x-www-form-urlencoded".

    static const Poco::Net::MediaType POST_CONTENT_TYPE_TEXT_PLAIN;
        ///< \brief A constant defining "text/plain".

private:
    PostRoute& _parent;
        ///< \brief The parent PostRoute reference.

};

    
} } // namespace ofx::HTTP
