# Projektisuunnitelma:

## Group g-059
Aleksi Könönen 
Antti Kainulainen
Lauri Hakavuori


Simple online store where developers can add JavaScript games and players can buy and play the games. The store has a payment service. One JavaScript game also implemented.

## Views:
  - Login with registeration
  - Browse games
  - Gamescreen with the games highscores
  - Purchases
  - Add game (as a developer)
  - User games
  - Succeeded purchase
  - Failed purchase

## Models:
  - Game
    - Highscores
    - URL
    - Developer (User)
    - Price
  - User
    - Type (player / developer)
    - Username
    - Password
    - User
    - Game
    - Timestamp

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
  - Categories for different game genres. (Browse games view)

###  Basic developer functionalities
  - Add a game (URL) and set price for that game and manage that game (remove,
    modify)
  - Basic game inventory and sales statistics. (User games view)
  - Security restrictions, e.g. developers are only allowed to modify/add/etc. their own games, developer can only add games to their own inventory (Implemented with django.contrib.auth)

###  Game/service interaction
  - When player has finished playing a game (or presses submit score), the game sends a postMessage to the parent window containing the current score. (Implemented with Game model)
  - Messages from service to the game. (Implemented with Game model)

###  Features that might be added:
  - 3rd party login with social media sharing
  - Mobile Friendly
  - Email validation
