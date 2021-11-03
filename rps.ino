#define MODE_SETUP 1
#define MODE_SELECTION 2
#define MODE_HIDDEN 3
#define MODE_COMBATREADY 4
#define MODE_COMBAT 5
#define MODE_SHOWSCORE 6
#define MODE_SHOWSCORING 7
#define MODE_GAMEOVER 8

#define RPS_SETUP 0
#define RPS_ROCK 1
#define RPS_SCISSOR 2
#define RPS_PAPER 3
#define RPS_GAMEOVER 4

#define WINNING_SCORE 7

#define SHOW_SCORE_DURATION 1000
#define SHOW_SCORING_DURATION 500
#define RESULT_PAUSE_DURATION 1000

Timer showScoreTimer;
Timer resultPauseTimer;

byte rps = RPS_ROCK;
byte opp = RPS_SETUP;

byte mode = MODE_SETUP;

byte scoreValues[3] = {0,0,0};

byte score = 0;

byte shownScoringVal = 0;
bool shownScoring = false;

byte contactFace = 0;

Color scoreColors[3] = {RED, YELLOW, GREEN};

void setup() {
  setColor(YELLOW);
  setValueSentOnAllFaces(RPS_SETUP);
  randomize();
  byte maxValue = random(2);
  for(byte i=0; i<3; i++) {
    byte pos = i + maxValue;
    while (pos>2) {
      pos = pos - 3;
    }
    scoreValues[pos] = 2-i;
  } 

}

void loop() {
  // put your main code here, to run repeatedly:
  if (mode==MODE_SETUP) {

    bool alone = true;
    FOREACH_FACE(f) {//check every face
      if (alone && !isValueReceivedOnFaceExpired(f)) {//no one there
        opp = getLastValueReceivedOnFace(f);
        contactFace = f;
        alone = false;
      }
    }
    if (opp==RPS_GAMEOVER) {
      mode = MODE_GAMEOVER;
      resultPauseTimer.set(RESULT_PAUSE_DURATION);
    }
    if (alone) {
      mode = MODE_HIDDEN;
    }
  }
  else if (mode==MODE_SELECTION) {
    if (buttonSingleClicked()) {
      rps++;
      if (rps>3) {
        rps=1;
      }
      drawUpdate(scoreColors[scoreValues[rps-1]],rps);
    }
    if (buttonDoubleClicked()) {
      mode = MODE_COMBATREADY;
      setValueSentOnAllFaces(rps);
      setColor(BLUE);  
    }

  }
  else if (mode==MODE_COMBATREADY) {
    FOREACH_FACE(face) {
      if (!isValueReceivedOnFaceExpired(face)) { //a neighbor!
        mode=MODE_COMBAT;
        contactFace = face;
      }
    }
    if (buttonSingleClicked() && mode==MODE_COMBATREADY) {
        mode=MODE_SELECTION;
        drawUpdate(scoreColors[scoreValues[rps-1]],rps);
    }
  }
  else if (mode==MODE_HIDDEN) {
    if (buttonSingleClicked()) {
      if(score>0) {
        mode=MODE_SHOWSCORE;
        setColor(OFF);
        showScoreTimer.set(SHOW_SCORE_DURATION);
        drawScore();
        
      } else if (!shownScoring) {
        mode=MODE_SHOWSCORING;
        showScoreTimer.set(SHOW_SCORING_DURATION);
        drawScoring(shownScoringVal);
//        drawUpdate(WHITE);
      } else {
        mode=MODE_SELECTION;
        drawUpdate(scoreColors[scoreValues[rps-1]],rps);
      }
    } else {
      setColor(YELLOW);
    }
  }
  else if (mode==MODE_COMBAT) {
    opp = getLastValueReceivedOnFace(contactFace);
    if(opp>RPS_SETUP) {
      if (rps!=opp) {
        bool victory = false;
        if(rps==(opp-1) || (rps==3 && opp==1)) {
          victory = true;
          drawUpdate(GREEN,rps);
          score = score + scoreValues[rps-1];
          mode=MODE_SETUP;
        }
        if(!victory) {
          drawUpdate(RED,rps);
          if (!isValueReceivedOnFaceExpired(contactFace)) { //a neighbor!
            opp = getLastValueReceivedOnFace(contactFace);
          }
          mode=MODE_SETUP;
        }
        if(score>=WINNING_SCORE) {
          mode = MODE_GAMEOVER;
          setValueSentOnFace(RPS_GAMEOVER, contactFace);
          resultPauseTimer.set(RESULT_PAUSE_DURATION);
        }
      } else {
          drawUpdate(WHITE,rps);
          mode = MODE_SETUP;
      }
    } else {
      mode = MODE_HIDDEN;
    }
  }
  else if (mode==MODE_SHOWSCORE) {
    if (showScoreTimer.isExpired()) {
      mode=MODE_SELECTION;
      drawUpdate(scoreColors[scoreValues[rps-1]],rps);
    }
  }
  else if (mode==MODE_SHOWSCORING) {
    if (showScoreTimer.isExpired()) {
      if(shownScoringVal>=2){
        mode=MODE_SELECTION;
        drawUpdate(scoreColors[scoreValues[rps-1]],rps);
        shownScoring = true;
      } else {
        shownScoringVal++;
        showScoreTimer.set(SHOW_SCORING_DURATION);
        drawScoring(shownScoringVal);
      }
    }
  }
  else if (mode==MODE_GAMEOVER) {
    if(resultPauseTimer.isExpired()) {
      if(score<WINNING_SCORE) {
        setColor(RED);
      } else {
        setColor(GREEN);
      }
    }
  }
}

void drawUpdate(Color color, byte val) {
  setColor(OFF);
  byte left = contactFace - 1;
  byte right = contactFace + 1;
  if (left<0) {
    left = 5;
  }
  if (right>5) {
    right = 0;
  }
  switch(val) {
    case RPS_SETUP:
      setColor(MAGENTA);
      //TODO: Flash? Let user know? Force default? Is this even possible?
      break;
    case RPS_ROCK:  
      setColorOnFace(color, contactFace);
      break;
    case RPS_SCISSOR:
      setColorOnFace(color, left);
      setColorOnFace(color, right);
      break;
    case RPS_PAPER:
      setColorOnFace(color, contactFace);
      setColorOnFace(color, left);
      setColorOnFace(color, right);     
      break;
    default:
      setColor(ORANGE);
      setColorOnFace(MAGENTA, val);
      break;
  }
}



void drawScore() {
  setColor(OFF);
  FOREACH_FACE(f) {
    if (f<score) {
      setColorOnFace(BLUE,f);
    }
  }  
}

void drawScoring(byte val) {
  drawUpdate(scoreColors[scoreValues[val]], val+1);
}
