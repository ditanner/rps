# RPS
Rock Paper Scissors prototype for the Blinks Game System

Uses an advanced variant - you score a different number of points depending on which choice you win with. This is set at random at the start of the game, and is (potentially) different for each player. The intention is to give the players an additional level of strategy.

Requires 2 Blinks, one for each player.

Rock is represented by a signle segment being lit, Scissors by two segments being lit with a gap inbetween (mimiking the two blades), and Paper is shown with 3 adjacent segments (i.e. a sheet). 

Once installed and ready, the blinks will show a yellow face. Each player takes a blink into their hand and interacts with it without the other player seeing.

The players will each click their blinks once, to signify that the opponent cannot see their blink. On the first round, the blink will then display the point value for each possible selection. Red denotes 0 points, Yellow denotes 1 point and Green denotes 2 points. On seubsequent rounds it will show the players score (if it is >0), signified by the number of blue segments displayed.

Following this, the blink will show the initial selection (which will be rock), in the colour that it is worth. The players may then click their blinks to change the value, and it will cycle through the 3 valid selections (displayed in their points values). Once the player has selected their choice, they double click and the Blink will show blue - this means it is ready for combat. Should the player wish to change their selection, they may click the blink again to got back into selection mode.

Once both players are ready for combat, the blinks are placed together. Both player choices will be displayed, with the winner in green and the loser in red. Points are added to the winning player's score in secret. If either player has a score of 7 (or more!), then they are the winner, and their blink will go greenm whilst the losers will go red. Otherwise the blinks can be seperated and play repeats. 
