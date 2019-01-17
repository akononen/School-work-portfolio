TIE-23506 Web Software Development
TIE-23500 Web-ohjelmointi

Spring 2018 course project  
Online game store for JavaScript games

Your group (student numbers, names, TUT emails)
 1. 253630, Aleksi Könönen, aleksi.kononen@student.tut.fi
 2. 239849, Antti Kainulainen, antti.kainulainen@student.tut.fi
 3. 256124, Lauri Hakavuori, lauri.hakavuori@student.tut.fi

# Final report and updating the project plan
Now it is time to update your project plan and write your final report. Read on for instructions!

## If you find problems with this document
 It is possible that this document has errors, or is not clear in its wording. If you have problems with this document email <mikko.nurminen@tut.fi> as soon as possible.

## Updating your project plan
Update your project plan to reflect what your group actually did, and how the work of your group progressed. Just be honest, this course is about learning. The project plan is in the README.md-file in your group's GitLab repository,

Report where do you feel that you were successful and where you had problems. Give sufficient details, this will in part influence the non-functional points awarded.

Report what choices you considered for the design and implementation of your game store and game. What were the reasons one option was chosen and others rejected?

Report how you divided the work between the team members - who did what. Was the work divided equally? If someone was supposed to do something, but didn’t, mark this down, too.

If a specific account/password (e.g. game developer) is required to try out and test some aspects of the work, please provide the details.

Make sure your names, emails and student IDs are in the document. Just saying this to make sure. :-)

Remember to commit your finalized project plan! It will be a part of the grading of your course project.

## Filling this final report
Save this file as final_report.md to the root directory of your group repo. The format is Markdown, and it shall remain so!

In this file, your group reports the features you have implemented, how you implemeted them, and how many points your group thinks the implementation is worth. You have to write a justification for each of the features listed! The feature list is the same as in the course project document.

For all the features and different parts of them the following questions (or some permutation of them) has to be answered:
- How and where in your code was the feature implemented?
- How many points your group thinks the implementation is worth?

Give enough information for the grading to happen based on your report. For features, name the modules, filenames, models, function, views etc. that are involved. As the time allocated for the grading is limited, only reported features will be checked.

#### Generic requirements  
###### Valid CSS and HTML
How does your code reflect this aspect? How did you validate?
###### The service should work on modern browsers
How does your code reflect this aspect? Tested on what browsers?
  - Service has been tested on Chrome, Firefox and Safari. The eventlistener on play-template
does not work on IE.
###### Code should be commented well
How does your code reflect this aspect? Give examples!
  - Code is easy to read by someone who has not made the code. For example update_highscores
function in views.py or games.html file.
###### Write your own code
How does your code reflect this aspect? Give examples!
  - We have written the code by ourselves. We took advantage of bootstrap and jquery libraries. I really do not know how to give examples about this.

#### Signs of quality
###### Reusability
How does your code reflect this aspect? Give examples!
  - This aspect could be improved. For example making methods and functions do only one thing. Still we have not repeated ourselves and there is no redundant code.
###### Modularity
How does your code reflect this aspect? Give examples!
  - Django itself give good base for modularity and we worked with it.
###### Versatile use of Django’s features
How does your code reflect this aspect? Give examples!
  - Django testing could have been improved but otherwise we used djangos features quite well. Forms, views and models for example.
###### Sensible URL scheme
How does your code reflect this aspect? Give examples!
  - URL scheme is logical and represantional. All urls are quite good example.
######  Security
How does your code reflect this aspect? Give examples!
  - Code uses django auth which is quite secure. Otherwise the different sites check user status and forbid him/her to do actions they shouldn't.
######  Crash & idiot proof
How does your code reflect this aspect? Give examples!
  - We haven't experienced any crashes on the service and it has been also tested by idiots.
###### Testing your game store
How does your code reflect this aspect? Give examples!
  - Django tests have not been exploited, but the service has been through excessive testing otherwise.

#### Mandatory requirements
##### Minimum functional requirements (mandatory)
###### Register as a player and developer
How and where in your code was the feature implemented?
  - Uses extended Django user model. Extended by makin boolena value is_developer
  which is used to separate players from developers.
##### Authentication (mandatory, 100-200 points)
How many points your group thinks the implementation is worth?
  - Authentication working without problems, so 100 points.
###### Login, logout and register, both as player or developer
How and where in your code was the feature implemented?
  - Front end implementation to navigation bar to better the user experience.
  Back end implementation with django forms, views and templates.
###### Email validation (max. +100 points)
How and where in your code was the feature implemented?
  - Not implemented.
#### Basic player functionalities (mandatory, 100-300 points)
How many points your group thinks the implementation is worth?
  - Basic player functionalities work well so 300 points.
###### As a player: how do players find games in your game store (are they in a category, is there a search functionality, etc.?)
How and where in your code was the feature implemented?
  - Players find their games in a table that can be sorted any way the user wants.
###### As a player: buy games (communication with Simple Paymenents, payment process verification)
How and where in your code was the feature implemented?
  - If player does not own a game, the user can buy it via the browse games page. After that user is directed to buy_game page and after that to the simplepayments service. After verification game is added to the users purchases.
###### As a player: play games
How and where in your code was the feature implemented?
  - Players can play games in the play page. The game comes to the service via iframe.
###### As a player: see game high scores and record their score
How and where in your code was the feature implemented?
  - Top 5 highscores are seen on the right side of the screen and are updated everytime player submits their score.
###### As a player: Security restrictions, e.g. player is only allowed to play the games they’ve purchased
How and where in your code was the feature implemented?
  - When user arrives to the play page the service checks that the plaer owns the game. Without writing urls by himself the user cannot get to the play page of games he does not own.

#### Basic developer functionalities (mandatory 100-200 points)
How many points your group thinks the implementation is worth?
  - Basic developer functionalities work well so 200 points.
###### As a developer: Add a game (URL) and set price for that game and manage that game (remove, modify)
How and where in your code was the feature implemented?
  - Adding and modifying games are impelented with django forms. Developer can also modify his own games as he likes or remove them completely.
###### As a developer: Basic game inventory and sales statistics (how many of the developers' games have been bought and when)
How and where in your code was the feature implemented?
  - Developers can see all the sales of their own games and when they have been bought.
###### As a developer: Security restrictions, e.g. developers are only allowed to modify/add/etc. their own games, developer can only add games to their own inventory, etc.
How and where in your code was the feature implemented?
  - Developers are only shown their own games so they cannot access the remove or modify pages of other games.

#### Game/service interaction (mandatory 100-200 points)
How many points your group thinks the implementation is worth?
  - 150 points.
###### Submitting high score's from the game using PostMessage
How and where in your code was the feature implemented?
  - Submitting highscores works with PostMessages. Highscores are shown to the player own play page.
###### Implementation of PostMessages from service to the game  
How and where in your code was the feature implemented?
  - Postmessages from service are handled, but all they contain is saving gamestate and we did not implement this non mandatory function.

#### Quality of Work (mandatory 100-200 points)  
How many points your group thinks the implementation is worth?
  - 150 points because we have not used django tests.
###### Quality of code (structure of the application, comments)
How does your code reflect this aspect? Give examples!
  - Code is of the highest quality and it is functions without problems.
###### Purposeful use of framework (Don't-Repeat-Yourself principle, Model-View-Template separation of concerns)
How does your code reflect this aspect? Give examples!
  - Django framework has been used smartly. Models, views and templates have clear separation and communicate with each other.
###### User experience (styling, interaction)
How does your code reflect this aspect? Give examples!
  - Whole service has been created with user experience in mind. Service is clear and easy to use and it is stylish.
###### Meaningful testing
How does your code reflect this aspect? Give examples!
  - Django tests have not been used, but the service has been tested excessively on clientside.

#### Non-functional requirements (mandatory 100-200 points)
How many points your group thinks the implementation is worth?
  - 200 points.

This covers your project plan (part of final grading, max. 50 points), as well as : "overall documentation, demo, teamwork, and project management as seen from the history of your GitLab project (and possible other sources that you submit in your final report)".  

You point out outstanding features of your project here. Much of this can be gathered from the project plan and repository statistics.

  - Documentation througout the project has been good. Commit messages has been informational excluding few merge commits. We have worked together as a team and most of our work has been done in the same space and we have tackled faced problems together.
  - Outstanding features:

#### Own JavaScript game(s) (mandatory 100-300  points)
How many points your group thinks the implementation is worth?
  - Game has gotten good reviews and the one relevant bug on postmessages has been fixed.
  - The point scale of the game is really imprecise so we ask for 250-300 points.
###### Your game(s) communicate with the service (the game has to use at least these three service features: high score, save, load)
How and where in your code was the feature implemented?
  - Game uses highscore, save and load features and a typo in save function has been fixed after the reviews.
###### Technical quality of the game (code, comments, communication with the game store)
How does your code reflect this aspect? Give examples!
  - Code is well commented and easy to understand.
###### Non-technical qualities of the game AKA the fun factor
Review your own!
  - As the reviews say, game is quite addictive which is a good thing for a game. Minor tweaks in the gameplay values may have made the game even more enjoyable, but still the game gives a feeling of pride and accomplishment to the player as he gets more clicks.

## Functionality that earns your group extra points

#### Save/load and resolution feature (0-100 points)
How and where in your code was the feature implemented?
How many points your group thinks the implementation is worth?
  - Not implemented.

#### 3rd party login (0-100 points)
How and where in your code was the feature implemented?
How many points your group thinks the implementation is worth?
  - Not implemented.
#### RESTful API (0-100 points)
How and where in your code was the feature implemented?
How many points your group thinks the implementation is worth?
  - Not implemented.
#### Mobile Friendly (0-50 points)
How and where in your code was the feature implemented?
How many points your group thinks the implementation is worth?
  - Service is mobile friendly, but some of the games are not. Still we think we should get 50 points.
#### Social media sharing (0-50 points)
How and where in your code was the feature implemented?
How many points your group thinks the implementation is worth?
  - Not implemented.

####  Some extra special feature your group has implemented (200 points max.)
If your group has implemented some extra feature(s), they can be considered for extra ponints, if impressive enough.
How and where in your code was the feature(s) implemented?
How many points your group thinks the implementation(s) are worth?

## After you have filled this file
In your group's GitLab repository:
- add this file as "final_report.md" to the root directory of your group repo
- commit it
- open an issue for this report with title "Final report ready"
- assign the newly created issue to WSD-Plussa

## That’s if for this course project. Now you can high-five and go out to the world as tested Django professionals!
