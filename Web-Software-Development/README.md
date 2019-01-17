# Project plan:

## Group g-059
Aleksi Könönen,
Antti Kainulainen,
Lauri Hakavuori 


Simple online store where developers can add JavaScript games and players can buy and play the games. The store has a payment service. One JavaScript game also implemented.

# URL for deployed Herokuapp: https://laalan.herokuapp.com/
## Views:
  - Login with registration
  - Browse games
  - Game screen with the games highscores
  - Add game (as a developer)
  - User games
  - Succeeded purchase
  - Failed purchase
  - Update highscores (not included in original project plan)


## Models:
  - Game
    - Highscores (This was made as an own model at the end of the project due to difficulties
      of sorting and editing json data)
    - URL
    - Developer (User)
    - Price
  - User
    - Type (player / developer)
    - Username
    - Password
  - Purchases
    - User
    - Game
    - Timestamp
  - Highscores (Not included in the original project plan)
    - Username
    - Game
    - Score

# Implemented features:

###  Minimum functional requirements
  - Register as a player and developers. (Which determines the type of User)
  - As a developer: add games to their inventory, see list of game sales.
  (Implemented with Purchases model)
  - As a player: buy games, play games, see game high scores and record their
  score to it. (Implemented with Purchases and Game models)


###  Authentication
  - Login, logout and register (both as player or developer). (Implemented with
    User model in Login with registeration view)



###  Basic player functionalities
  - Buy games, payment is handled by a mockup payment service. (Implemented with
    Purchases model)
  - Play games. (Gamescreen view)
  - Security restrictions, e.g. player is only allowed to play the games they’ve
   purchased. (Implemented with django.contrib.auth)
  - Categories for different game genres. (Browse games view) (We did not do Categories
    but instead a table that can be sorted any way the user wants, and the table also
    has a search functionality.)



###  Basic developer functionalities
  - Add a game (URL) and set price for that game and manage that game (remove,
    modify)
  - Basic game inventory and sales statistics. (User games view)
  - Security restrictions, e.g. developers are only allowed to modify/add/etc. their own games, developer can only add games to their own inventory (Implemented with django.contrib.auth)

###  Game/service interaction
  - When player has finished playing a game (or presses submit score), the game sends a postMessage to the parent window containing the current score. (Implemented with Game model)
  - Messages from service to the game. (Implemented with Game model)


###  Features that might be added:
  - 3rd party login with social media sharing (was not a part of final project)
  - Mobile Friendly
  - Email validation (was not a part of final project)


# Reflection to project

## Implemented features:
The original project plan is edited so it shows which features were actually implemented and which implementations or plans cahnged during project.

## Groups workload division
  - Aleksi Könönen
    - own javascript game
    - games template and functionality
    - base template and functionality
    - home template and functionality
    - play template and functionality
    - highscore updating
  - Antti Kainulainen
    - Purchased/Developed games template and functionality
    - Sales template and functionality
    - Editing/removing games functionality
    - Sketching of the used models
    - Game view HTML/CSS
  - Lauri Hakavuori
    - Game adding functionality and form
    - Creation and setup of Heroku
    - Parts of playview and html
    - Dev/player part of user creation
    - Dev/player separation for html pages

## Project progression

One of the largest problems we had was with setting up the work environment especially
with setting up PostgreSQL databases. Starting the project and making the homepage
was also problematic but once we got the hang of it the project ran mostly smoothly.

Approaching the end of the project we encountered problems with our original
model structure. We found out that storing highscores as json data in Game model
made updating and reading the data really hard so we made new model for the highscores.
(We probably should have listened more closely to the feedback we got from the project plan
but were too stubborn.)

We did most of the work working in the same space and tackled the rising problems together.

## Using the service

The service is pretty straight forward to use and user experience has been taken into consideration.
If the user has used web services before we dont expect that the user faces problems with it.
Service requires a login as it states on the homepage. On account creation user has the
option to create a develeloper(can add games to the service) or a player (can buy
and play games) account.

After creating the account the user can add games or buy and play games.
