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


#include "ofApp.h"


void ofApp::setup()
{
    ofSetFrameRate(30);

    bgColor = ofColor::white;

    BasicWebSocketServerSettings settings;

    server = BasicWebSocketServer::makeShared(settings);

    server->getWebSocketRoute()->registerWebSocketEvents(this);
    server->start();

    ofx::Zeroconf::Manager::registerClientEvents(this);
    ofx::Zeroconf::Manager::registerBrowseEvents(this);

    browseRef = Browser::defaultBrowser().browse("_http._tcp");

    Zeroconf::Service service("_http._tcp.", server->getSettings().getPort());

    serviceRef = Client::defaultClient().registerService(service);

    // Launch a browser with the address of the server.
    ofLaunchBrowser(server->getURL());
}

void ofApp::update()
{
    Json::Value setCurrentFrameJSON;
    setCurrentFrameJSON["method"] = "setCurrentFrame";
    Json::Value setCurrentFrameParamsJSON;
    setCurrentFrameParamsJSON["frameNum"] = ofGetFrameNum();
    setCurrentFrameJSON["data"] = setCurrentFrameParamsJSON;

    server->getWebSocketRoute()->broadcast(setCurrentFrameJSON.toStyledString());
}

//------------------------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(bgColor);
    ofSetColor(255);
    
    std::size_t numConnected = server->getWebSocketRoute()->getNumWebSocketConnections();

    ofSetColor(0);

    ofDrawBitmapStringHighlight("Num Connected: " + ofToString(numConnected),ofPoint(40,40));

}

//------------------------------------------------------------------------------
void ofApp::onWebSocketOpenEvent(WebSocketEventArgs& evt)
{
    cout << "Connection opened from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
}

//------------------------------------------------------------------------------
void ofApp::onWebSocketCloseEvent(WebSocketEventArgs& evt)
{
    cout << "Connection closed from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
}

//------------------------------------------------------------------------------
void ofApp::onWebSocketFrameReceivedEvent(WebSocketFrameEventArgs& evt)
{
    cout << "Frame from: " << evt.getConnectionRef().getClientAddress().toString() << endl;

    ofxJSONElement json;

    if(json.parse(evt.getFrameRef().getText()))
    {
        std::cout << json.toStyledString() << std::endl;

        if(json.isMember("command") && json["command"] == "SET_BACKGROUND_COLOR")
        {
            if(json["data"] == "white")
            {
                bgColor = ofColor::white;
            }
            else if(json["data"] == "black")
            {
                bgColor = ofColor::black;
            }
            else
            {
                cout << "Unknown color: " << json["data"].toStyledString() << endl;
            }
        }
    }
    else
    {
        ofLogError("ofApp::onWebSocketFrameReceivedEvent") << "Unable to parse JSON: "  << evt.getFrameRef().getText();
    }
}

//------------------------------------------------------------------------------
void ofApp::onWebSocketFrameSentEvent(WebSocketFrameEventArgs& evt)
{
    // frame was sent to clients
}

//------------------------------------------------------------------------------
void ofApp::onWebSocketErrorEvent(WebSocketEventArgs& evt)
{
    cout << "Error from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
}


//------------------------------------------------------------------------------
void ofApp::onZeroconfServiceRegistered(const Zeroconf::ClientEventArgs& args)
{
    cout << "SERVICE REGISTERED" << endl;
    cout <<  ofToString(args.service) << endl;
}

//------------------------------------------------------------------------------
void ofApp::onZeroconfServiceRegistrationFailed(const Zeroconf::ClientErrorEventArgs& args)
{
    cout << "SERVICE REGISTERATION FAILED" << endl;
    cout << "Error: " << args.error.getMessage() << endl;
    cout <<  ofToString(args.service) << endl;
}

//------------------------------------------------------------------------------
void ofApp::onZeroconfBrowseError(const Zeroconf::BrowseErrorEventArgs& args)
{
    cout << "BROWSER ERROR: " << args.error.getMessage() << endl;
}

//------------------------------------------------------------------------------
void ofApp::onZeroconfBrowseServiceFound(const Zeroconf::BrowseServiceEventArgs& args)
{
    cout << "SERVICE FOUND" << endl;
    cout <<  ofToString(args.service) << endl;
    //        ofxZeroconf::resolve(args.service);
}

//------------------------------------------------------------------------------
void ofApp::onZeroconfBrowseServiceRemoved(const Zeroconf::BrowseServiceEventArgs& args)
{
    cout << "SERVICE REMOVED" << endl;
    cout <<  ofToString(args.service) << endl;
}

//------------------------------------------------------------------------------
void ofApp::onZeroconfResolveError(const Zeroconf::BrowseErrorEventArgs& args)
{
    cout << "RESOLVE ERROR" << endl;
    cout <<  ofToString(args.error.getMessage()) << endl;
}

//------------------------------------------------------------------------------
void ofApp::onZeroconfServiceResolved(const Zeroconf::BrowseServiceEventArgs& args)
{
    cout << "SERVICE RESOLVED" << endl;
    cout <<  ofToString(args.service) << endl;

    // ofxZeroconf::resolveHost(args.service.getHost());
}

//------------------------------------------------------------------------------
void ofApp::onZeroconfHostResolveError(const Zeroconf::BrowseErrorEventArgs& args)
{
    cout << "HOST RESOLVE ERROR" << endl;
    cout <<  ofToString(args.error.getMessage()) << endl;
}

//------------------------------------------------------------------------------
void ofApp::onZeroconfHostResolved(const Zeroconf::ResolveHostEventArgs& args)
{
    cout << "HOST RESOLVED" << endl;
    cout <<  ofToString(args.ipAddress.toString()) << endl;
    cout <<  ofToString(args.ttl) << endl;
    cout <<  ofToString(args.options) << endl;
}

