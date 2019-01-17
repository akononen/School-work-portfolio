"""wsd2018project URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.0/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""

from django.contrib import admin
from django.urls import path, include
import WebStore.views
import users.urls
from django.views.generic.base import TemplateView

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', TemplateView.as_view(template_name='home.html'), name='home'),

    # Home sisältää pelien selaamisen
    path('home/', WebStore.views.home, name='home'),

    path('games/', WebStore.views.games, name='games'),

    path('users/', include('users.urls')),
    path('users/', include('django.contrib.auth.urls')),

    path('profile/', WebStore.views.profile, name='profile'),
    path('profile/addgame/', WebStore.views.addgame, name='addgame'),
    path('buygame/<int:gameid>/', WebStore.views.buygame, name='buygame'),

    path('gamebought/<int:gameid>/', WebStore.views.gamebought, name='gamebought'),
    path('play/<int:gameid>', WebStore.views.play, name='play'),
    path('deletegame/<int:gameid>', WebStore.views.deletegame, name='deletegame'),
    path('editgame/<int:gameid>', WebStore.views.editgame, name='editgame'),
    path('gamesales/<int:gameid>', WebStore.views.gamesales, name='gamesales'),
    path('update_highscores/', WebStore.views.update_highscores, name='update_highscores')
]
