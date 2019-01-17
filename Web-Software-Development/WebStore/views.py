from django.shortcuts import render, redirect
from django.http import HttpResponseRedirect, Http404
from django.http import JsonResponse
from django.contrib.postgres.fields import JSONField
from django.views import generic
from .models import Game, Purchases, Highscores
from users.models import CustomUser
from .forms import AddGameForm
from hashlib import md5
import json
from django.db.models import Sum
from django.core import serializers


def login(request):
    return render(request, "login.html")


def home(request):
    #return HttpResponse("home page")
    return render(request, "home.html")


def profile(request):
    purchased_games = Purchases.objects.filter(user = request.user)
    developed_games = Game.objects.filter(developer = request.user)

    return render(request, "profile.html", { 'purchased_games' : purchased_games,
                                                'developed_games' : developed_games })


def addgame(request):
    # if this is a POST request we need to process the form data
    if request.method == 'POST':

        # create a form instance and populate it with data from the request:
        form = AddGameForm(request.POST)
        #form.developer = request.user.id# check whether it's valid:
        if form.is_valid():

            uusipeli = form.save(commit=False)
            uusipeli.developer_id = request.user.id  # The logged-in user
            uusipeli.save()

            return HttpResponseRedirect('/profile/')

    # if a GET (or any other method) we'll create a blank form
    else:
        form = AddGameForm()

    return render(request, 'addgame.html', {'form': form})


def buygame(request, gameid):

    selectedgame = Game.objects.get(id=gameid)

    # Tähän haettava viimeinen purchase-id+1
    pid = "123456"
    sid = "G059"
    amount = selectedgame.price
    secret_key = "a5d6cdd4cc1129106825e94041de749d"

    checksumstr = "pid={}&sid={}&amount={}&token={}".format(pid, sid, amount, secret_key)
    m = md5(checksumstr.encode("ascii"))
    checksum = m.hexdigest()
    selectedgame = Game.objects.get(id=gameid)
    return render(request, "buygame.html", {'game' : selectedgame, 'checksum' : checksum})


def games(request):

    game_list = Game.objects.all()
    purchase_list = Purchases.objects.filter(user = request.user)
    purchased_games = []
    for purchase in purchase_list:
        purchased_games.append(purchase.game)

    return render(request, "games.html", {'game_list': game_list,
                                            'purchase_list': purchased_games})


def gamebought(request, gameid):
    user = request.user.id
    game = Game.objects.get(id = gameid)

    purchase = Purchases(user=request.user, game=game, price=game.price)
    purchase.save()

    return redirect('/games/')


def play(request, gameid):
    game = Game.objects.get(id = gameid)
    users = CustomUser.objects.all()
    user = request.user
    purchase_list = Purchases.objects.filter(user = request.user)
    purchased_games = []
    for purchase in purchase_list:
        purchased_games.append(purchase.game)


    highscores = Highscores.objects.filter(game=game).order_by('-score')[:5]
    # purchased = False
    # for purchase in purchase_list:
    #     if game.id == purchase.game.id:
    #         purchased = True
    return render(request, "play.html", {'game': game, 'purchased_games': purchased_games, 'highscores' : highscores})



def deletegame(request, gameid):
    game = Game.objects.get(id = gameid)
    game.delete()

    return redirect('/profile/')


def gamesales(request, gameid):
    game = Game.objects.get(id = gameid)
    sales = Purchases.objects.filter(game = game)
    totalsales = Purchases.objects.filter(game = game).aggregate(Sum('price'))

    return render(request, "gamesales.html", {'game': game, 'sales': sales,
                                            'totalsales' : totalsales})


def editgame(request, gameid):

    game = Game.objects.get(id = gameid)

    if request.method == 'POST':

        form = AddGameForm(request.POST)

        if form.is_valid():

            game.name = request.POST.get('name')
            game.url = request.POST.get('url')
            game.price = request.POST.get('price')
            game.save()

            return HttpResponseRedirect('/profile/')
    else:
        form = AddGameForm(instance=game)

    return render(request, 'editgame.html', {'form': form, 'game' : game })



    #return render(request, "editgame.html", {'game': game})

def update_highscores(request):


    user = request.GET.get('user')
    game = request.GET.get('game')
    score = float(request.GET.get('score'))

    user_highscores = Highscores.objects.filter(game = game, username = user)

    #handling the score and saving it if it is better than earlier score
    if len(user_highscores) == 0:
        highscore = Highscores(game=Game.objects.get(id=game), username=user, score=score)
        highscore.save()
    else:
        if user_highscores[0].score < score:
            user_highscores[0].score = score
            user_highscores[0].save()

    highscores = serializers.serialize('json', Highscores.objects.filter(game=game).order_by('-score')[:5])

    return JsonResponse(highscores, safe=False)
