/*
LyapkiVR by Endless Attractions Museum
for Processing

Play online: https://openprocessing.org/sketch/1100098

//--------------------------------------------------
Description:
//--------------------------------------------------
This is a 3D game consisting of you and a single point running from you. 
You must catch the point. If succeed - you will see a flash.

//--------------------------------------------------
How to play:
//--------------------------------------------------
Move mouse to rotate left and right and try to follow the point . 
Press Space to restart the game. 
Press 1,2 to switch 2d/3d view.
*/

//enemy
PVector e_pos_;
float  e_vel_ = 0;
float  e_angle_ = 0;
float e_angle_target_ = 0;
float e_angle_vel_ = 360;    //rotation speed

float e_time_rotate_ = 0;
//my
PVector my_pos_;
float my_angle_ = 0;
float my_angle_target_ = 0;
float my_vel_ = 0;
float my_angle_vel_ = 120;    //rotation speed

float dist_ = 0;
float flash_phase_ = 0;
float flash_period_ = 1.0;
float flash_time_ = 0;  //last flash start
boolean e_visible_ = false;  //is enemy visible (because of flashing)

//params
float e_vel0 = 10;
float my_vel0 = 12;
float time_e_rotate0 = 0.5;  //period, seconds, between enemy changes direction

float dt = 1.0 / 30.0;

float e_catch_dist = 2;
float e_far_dist = 30;

//draw mode
int draw_mode_ = 1;  //0 - 2d, 1 - vr


float time0;

//--------------------------------------------------------------
//Utilities

float elapsed_time() {
  return millis() * 0.001;
}


PVector get_dir(float angle) {
  return PVector.fromAngle(angle * DEG_TO_RAD);
}

float rotate_to(float current, float target, float move, boolean wrap) {
  if (wrap) {
    if (abs(target + 360 - current) < abs(target - current)) target += 360;
    if (abs(target - 360 - current) < abs(target - current)) target -= 360;
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

//--------------------------------------------------------------
void new_game() {
 //new game
  my_pos_ = new PVector(0,0);
  my_angle_ = 0;
  my_angle_target_ = 0;
  my_vel_ = my_vel0;

  e_pos_ = new PVector(10, random(-8, 8));
  e_angle_ = PVector.angleBetween((e_pos_),new PVector(1,0)) * RAD_TO_DEG; //run away
  e_angle_target_ =e_angle_;
  e_vel_ = e_vel0;
  e_time_rotate_ = elapsed_time();

  flash_time_ = elapsed_time();
  
  //print("e_angle_", e_angle_);
}


//--------------------------------------------------------------
//rotate from mouse
void game_rotate(float delta_pix) {
  my_angle_target_ += delta_pix;      //mouse speed
}

//--------------------------------------------------------------
float mapf_constr(float x, float a, float b, float A, float B) {
   x = (x-a)/(b-a)*(B-A)+A;
   x = min(max(x,min(A,B)),max(A,B));
   return x;
}

//--------------------------------------------------------------
void game_update() {
  float time = elapsed_time();
  float dt = time-time0;
  time0 = time;

 //enemy changes direction
  if (time >= e_time_rotate_ + time_e_rotate0) {
    e_time_rotate_ = time;

    float away_angle = PVector.angleBetween((e_pos_),new PVector(1,0)) * RAD_TO_DEG; //run away
    e_angle_target_ = away_angle + random(my_angle_ - 90, my_angle_ + 90);   //+= ofRandom(-135, 135);
  }

  //rotate
  e_angle_ = rotate_to(e_angle_, e_angle_target_, dt * e_angle_vel_, true);
  my_angle_ = rotate_to(my_angle_, my_angle_target_, dt * my_angle_vel_, false);

  
  //move
  PVector mdir = get_dir(my_angle_);
  my_pos_.x += mdir.x * my_vel0 * dt;
  my_pos_.y += mdir.y * my_vel0 * dt;
  //mdir.mult(my_vel0 * dt);
  //my_pos_.add(mdir);
  
  PVector edir = get_dir(e_angle_);
  e_pos_.x += edir.x * e_vel0 * dt;
  e_pos_.y += edir.y * e_vel0 * dt;
  
  //edir.mult(e_vel0 * dt);
  //e_pos_.add(edir);

  
  //flash phase depends on the distance
  dist_ = dist(my_pos_.x, my_pos_.y, e_pos_.x, e_pos_.y);
  float freq = mapf_constr(dist_, e_catch_dist, e_far_dist, 10, 0.3); //flash frequency formula
  flash_period_ = 1.0 / freq;


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
}


//--------------------------------------------------------------
void draw_2d() {
  float w = width;
  float h = height;
  pushMatrix();
  translate(w / 2, h / 2);
  scale(5, 5);
  scale(1,1);
  translate(-my_pos_.x, -my_pos_.y);
  fill(255);
  
  //me
  ellipse(my_pos_.x, my_pos_.y, 1,1); //0.1,0.1);

  PVector my_dir = get_dir(my_angle_); 
  stroke(255);
  line(my_pos_.x, my_pos_.y, my_pos_.x + 5*my_dir.x, my_pos_.y + 5*my_dir.y);        

  //enemy
  if (e_visible_) {
    ellipse(e_pos_.x, e_pos_.y, 3,3); //0.1,0.1);
    //ofLine(e_pos_, e_pos_ + get_dir(e_angle_));
  }


  //fSetColor(128);
  //NoFill();
  //ofCircle(my_pos_, e_catch_dist);
  //ofCircle(my_pos_, e_far_dist);

  //ofSetColor(255);
  popMatrix();

  //if close - fill the screen
  if (dist_ < e_catch_dist) {
    fill(200);
    rect(0, 0, w, h);
  }

  //ofSetColor(255);
  //ofDrawBitmapString("dist " + ofToString(dist_), 20, 20);
}

//--------------------------------------------------------------
void draw_vr() {
  float w = width;
  float h = height;


  //---------------
  //draw_2d();
  //ofSetColor(0, 160);
  //ofFill();
  //ofDrawRectangle(0, 0, w, h);
  //---------------

  PVector d0 = new PVector();
  d0.x = e_pos_.x - my_pos_.x;
  d0.y = e_pos_.y - my_pos_.y;
  
  float myang = my_angle_target_ * DEG_TO_RAD;
  //rotate
  float dx = d0.x * cos(-myang) - d0.y * sin(-myang);
  float dy = d0.y * cos(-myang) + d0.x * sin(-myang);

  //draw
  
  pushMatrix();
  translate(w / 2, h / 2);
  
  //enemy
  if (e_visible_) {
    if (dx > 0) {
      fill(255);
      float posx = dy / (0.2 + dx) * w/5;   //main 3d formula
      rect(int(posx)-1, -1, 3, 3);
      //ofLine(0, 0, posx, 0);
    }
  }

  //geom debug
  //ofSetColor(255, 0, 0);
  //ofLine(0, 0, d.x * 15, d.y * 15);

  popMatrix();

  //if close - fill the screen
  if (dist_ < e_catch_dist) {
    fill(200);
    rect(0, 0, w, h);
  }

  //ofSetColor(255);
  //ofDrawBitmapString("dist " + ofToString(dist_), 20, 20);
}
//--------------------------------------------------------------
void setup() {
  size(800,600);
  time0 = elapsed_time();
  
  new_game();
} 

//--------------------------------------------------------------
void draw() {

  game_update();
  
  background(0);  
  if (draw_mode_ == 0) draw_2d();
  if (draw_mode_ == 1) draw_vr();
  
  float mouse = float(mouseX - width/2)/width;
  game_rotate(mouse*10);
  //println(mouse);
  
  //if (time>time0+1) background(100);
  //ellipse(mouseX, mouseY, 20, 20);
  
  //println("dist_", dist_, "visible", e_visible_);
}

int mouse0 = 0;
//--------------------------------------------------------------
void mousePressed() {
//  mouse0 = mouseX;
  //println("pressed");
}

//--------------------------------------------------------------
void mouseDragged() {
  //game_rotate(mouseX - mouse0);
  //mouse0 = mouseX;
  //println("dragged");

}

void keyPressed() {
   if (key == ' ') {
      new_game(); 
   }
   if (key == '1') {
      draw_mode_ = 0; //2d 
   }
   if (key == '2') {
      draw_mode_ = 1; //vr
   }
}

//--------------------------------------------------------------


//--------------------------------------------------------------
