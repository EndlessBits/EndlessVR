#include "ofApp.h"

//enemy
ofPoint e_pos_;
float  e_vel_ = 0;
float  e_angle_ = 0;
float e_angle_target_ = 0;
float e_angle_vel_ = 360;		//rotation speed

float e_time_rotate_ = 0;
//my
ofPoint my_pos_;
float my_angle_ = 0;
float my_angle_target_ = 0;
float my_vel_ = 0;
float my_angle_vel_ = 120;		//rotation speed

float dist_ = 0;
float flash_phase_ = 0;
float flash_period_ = 1.0;
float flash_time_ = 0;	//last flash start
bool e_visible_ = false;	//is enemy visible (because of flashing)

//params
float e_vel0 = 10;
float my_vel0 = 12;
float time_e_rotate0 = 0.5;	//period, seconds, between enemy changes direction

float dt = 1.0 / 30.0;

float e_catch_dist = 2;
float e_far_dist = 30;

//draw mode
int draw_mode_ = 1;	//0 - 2d, 1 - vr


//--------------------------------------------------------------
void new_game() {
	my_pos_ = ofPoint(0);
	my_angle_ = 0;
	my_angle_target_ = 0;
	my_vel_ = my_vel0;

	e_pos_ = ofPoint(10, ofRandom(-8, 8));
	e_angle_ = -ofVec2f(e_pos_ - my_pos_).angle(ofVec2f(1,0)); //run away
	e_angle_target_ = e_angle_; 
	e_vel_ = e_vel0;
	e_time_rotate_ = ofGetElapsedTimef();

	flash_time_ = ofGetElapsedTimef();
}

ofPoint get_dir(float angle) {
	return ofPoint(cos(angle * DEG_TO_RAD), sin(angle * DEG_TO_RAD));
}

float rotate_to(float current, float target, float move, bool wrap) {
	if (wrap) {
		if (fabs(target + 360 - current) < fabs(target - current)) target += 360;
		if (fabs(target - 360 - current) < fabs(target - current)) target -= 360;
	}

	//move
	if (current < target) {
		current = min(current + move, target);	
	}
	else {
		current = max(current - move, target);
	}
	return current;
}

void game_update() {
	//enemy changes direction
	float time = ofGetElapsedTimef();
	if (time >= e_time_rotate_ + time_e_rotate0) {
		e_time_rotate_ = time;

		float away_angle = -ofVec2f(e_pos_ - my_pos_).angle(get_dir(my_angle_));
		e_angle_target_ = away_angle + ofRandom(my_angle_ - 90, my_angle_ + 90);   //+= ofRandom(-135, 135);
	}

	//rotate
	e_angle_ = rotate_to(e_angle_, e_angle_target_, dt * e_angle_vel_, true);
	my_angle_ = rotate_to(my_angle_, my_angle_target_, dt * my_angle_vel_, false);

	//move

	my_pos_ += get_dir(my_angle_) * my_vel0 * dt;
	e_pos_ += get_dir(e_angle_) * e_vel0 * dt;

	//flash phase depends on the distance
	dist_ = ofDist(my_pos_.x, my_pos_.y, e_pos_.x, e_pos_.y);
	float freq = ofMap(dist_, e_catch_dist, e_far_dist, 10, 0.3, true); //flash frequency formula
	flash_period_ = 1.0 / freq;
	//cout << "dist " << dist << "   freq " << freq << "   period " << flash_period_ << endl;

	//flash_phase_ += freq * dt;
	//flash_phase_ = fmod(flash_phase_, 1);

	//flashing
	float ftime = time - flash_time_;
	if (e_visible_) {
		if (ftime > flash_period_) {
			e_visible_ = false;
			flash_time_ = time;
		}
	}
	else {
		if (ftime > min(flash_period_/2, 0.1f)) {
			e_visible_ = true;
			flash_time_ = time;
		}
	}

	//cout << "my " << my_pos_.x << " " << my_pos_.y << ",  en " << e_pos_.x << " " << e_pos_.y << endl;
}

void game_rotate(float delta_pix) {
	my_angle_target_ += delta_pix / //5.0;			//mouse speed
		10.0; 
}

void draw_2d() {
	float w = ofGetWidth();
	float h = ofGetHeight();
	ofPushMatrix();
	ofTranslate(w / 2, h / 2);
	ofScale(15, 15);
	ofTranslate(-my_pos_);
	//ofRotate(-my_angle_);
	ofSetColor(255);
	ofFill();
	
	//me
	ofCircle(my_pos_, 0.1);

	ofLine(my_pos_, my_pos_ + 0.5*get_dir(my_angle_));				//real speed
	ofLine(my_pos_, my_pos_ + 1*get_dir(my_angle_target_));	//line of sight
	ofSetColor(255);

	//enemy
	if (e_visible_) {
		ofCircle(e_pos_, 0.1);
		ofLine(e_pos_, e_pos_ + get_dir(e_angle_));
	}


	ofSetColor(128);
	ofNoFill();
	ofCircle(my_pos_, e_catch_dist);
	ofCircle(my_pos_, e_far_dist);

	ofSetColor(255);
	ofPopMatrix();

	//if close - fill the screen
	if (dist_ < e_catch_dist) {
		ofSetColor(200);
		ofFill();
		ofDrawRectangle(0, 0, w, h);
	}

	ofSetColor(255);
	ofDrawBitmapString("dist " + ofToString(dist_), 20, 20);
}

void draw_vr() {
	float w = ofGetWidth();
	float h = ofGetHeight();


	//---------------
	//draw_2d();
	//ofSetColor(0, 160);
	//ofFill();
	//ofDrawRectangle(0, 0, w, h);
	//---------------

	ofPoint d0 = e_pos_ - my_pos_;
	float myang = my_angle_target_ * DEG_TO_RAD;
	//rotate
	ofPoint d;
	d.x = d0.x * cos(-myang) - d0.y * sin(-myang);
	d.y = d0.y * cos(-myang) + d0.x * sin(-myang);

	//draw
	
	ofPushMatrix();
	ofTranslate(w / 2, h / 2);
	
	//enemy
	if (e_visible_) {
		if (d.x > 0) {
			ofSetColor(255);
			ofFill();
			float posx = d.y / (0.2 + d.x) * w/5;   //main 3d formula
			ofDrawRectangle(int(posx)-1, -1, 3, 3);
			//ofLine(0, 0, posx, 0);
		}
	}

	//geom debug
	//ofSetColor(255, 0, 0);
	//ofLine(0, 0, d.x * 15, d.y * 15);

	ofPopMatrix();

	//if close - fill the screen
	if (dist_ < e_catch_dist) {
		ofSetColor(200);
		ofFill();
		ofDrawRectangle(0, 0, w, h);
	}

	ofSetColor(255);
	ofDrawBitmapString("dist " + ofToString(dist_), 20, 20);

}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(30);
	ofSetCircleResolution(40);

	ofBackground(34, 34, 34);

	int bufferSize = 512;
	sampleRate = 44100;
	phase = 0;
	phaseAdder = 0.0f;
	phaseAdderTarget = 0.0f;
	volume = 0.05f;
	bNoise = false;

	soundStream.printDeviceList();

	ofSoundStreamSettings settings;

	// if you want to set the device id to be different than the default:
	//	auto devices = soundStream.getDeviceList();
	//	settings.setOutDevice(devices[3]);
	// you can also get devices for an specific api:
	//	auto devices = soundStream.getDeviceList(ofSoundDevice::Api::PULSE);
	//	settings.setOutDevice(devices[0]);

	auto devices = soundStream.getDeviceList(ofSoundDevice::Api::MS_DS);
	settings.setOutDevice(devices[0]);

	settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);


	new_game();

}


//--------------------------------------------------------------
void ofApp::update() {
	game_update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0);
	if (draw_mode_ == 0) draw_2d();
	if (draw_mode_ == 1) draw_vr();

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == '1') draw_mode_ = 0;	//2d
	if (key == '2') draw_mode_ = 1;	//vr
	if (key == ' ') new_game();	//restart

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

int mouse0 = 0;
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	game_rotate(x - mouse0);
	mouse0 = x;
}

//--------------------------------------------------------------

void ofApp::mousePressed(int x, int y, int button) {
	mouse0 = x;
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& buffer) {
	//pan = 0.5f;
	float leftScale = 1 - pan;
	float rightScale = pan;

	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-TWO_PI like this:
	while (phase > TWO_PI) {
		phase -= TWO_PI;
	}


	phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
	for (size_t i = 0; i < buffer.getNumFrames(); i++) {
		phase += phaseAdder;
		float sample = sin(phase);
		buffer[i * buffer.getNumChannels()] = sample * volume * leftScale;
		buffer[i * buffer.getNumChannels() + 1] = sample * volume * rightScale;

	}

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
