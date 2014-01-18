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


#include "ofx/HTTP/Server/PostRouteHandler.h"
#include "ofx/HTTP/Server/PostRoute.h"
#include "ofx/HTTP/Utils/Utils.h"
#include "Poco/UUID.h"
#include "Poco/UUIDGenerator.h"


namespace ofx {
namespace HTTP {


const Poco::Net::MediaType PostRouteHandler::POST_CONTENT_TYPE_TEXT_PLAIN = Poco::Net::MediaType("text/plain");
const Poco::Net::MediaType PostRouteHandler::POST_CONTENT_TYPE_MULTIPART  = Poco::Net::MediaType("multipart/form-data");
const Poco::Net::MediaType PostRouteHandler::POST_CONTENT_TYPE_URLENCODED = Poco::Net::MediaType("application/x-www-form-urlencoded");


PostRouteHandler::PostRouteHandler(PostRoute& parent):
    _parent(parent)
{
}


PostRouteHandler::~PostRouteHandler()
{
}


void PostRouteHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                     Poco::Net::HTTPServerResponse& response)
{
    Poco::UUID formUUID = Poco::UUIDGenerator::defaultGenerator().createOne();

    // get the content type header (already checked in parent route)
    Poco::Net::MediaType contentType(request.get("Content-Type", ""));

    if (contentType.matches(POST_CONTENT_TYPE_URLENCODED) ||
        contentType.matches(POST_CONTENT_TYPE_MULTIPART))
    {
        // prepare the upload directory if needed
        if (contentType.matches(POST_CONTENT_TYPE_MULTIPART))
        {
            ofDirectory _uploadFolder(_parent.getSettings().getUploadFolder());

            if(!_uploadFolder.exists())
            {
                ofLogError("ServerUploadRouteHandler::handleRequest") << "Upload folder does not exist and cannot be created.";
                response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
                _parent.handleRequest(request,response);
                return;
            }
        }

        PostRouteFileHandler postRoutePartHandler(request, _parent, formUUID.toString());

        Poco::Net::HTMLForm form(contentType.toString());
        form.setFieldLimit(_parent.getSettings().getFieldLimit());
        form.load(request, request.stream(), postRoutePartHandler);

        HTTPFormEventArgs args(request,
                               formUUID.toString(),
                               form);

        ofNotifyEvent(_parent.events.onHTTPFormEvent, args, &_parent);
    }
    else if (contentType.matches(POST_CONTENT_TYPE_TEXT_PLAIN))
    {
        // Poco::Net::HTMLForm, like php does not handle text/plain because
        // it cannot be unambiguously encoded.  Here we simply return
        // the raw text with the event.

        std::string result;
        Poco::StreamCopier::copyToString(request.stream(), result);

        HTTPRawFormEventArgs args(request,
                                  formUUID.toString(),
                                  result);

        ofNotifyEvent(_parent.events.onHTTPRawFormEvent,
                      args,
                      &_parent);
    }
    else
    {
        // error events are handled at a deeper level
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR,
                                    "Unknown Content-Type: " + request.get("Content-Type"));
        _parent.handleRequest(request, response);

        return;
    }

    if(!_parent.getSettings().getUploadRedirect().empty())
    {
        response.redirect(_parent.getSettings().getUploadRedirect());
    }
    else
    {
        // done
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentLength(0);
        response.send();
    }

}
    
} } // namespace ofx::HTTP
