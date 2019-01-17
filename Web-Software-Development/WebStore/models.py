from django.db import models
from django.contrib.postgres.fields import JSONField
#from django.contrib.auth.models import CustomUser
#from WebStore import CustomUser
# Create your models here.

# class User(models.User):
#     developer = BooleanField()
#     username =
# tehdäänkö custom model vai käytetäänkö valmista User luokkaa?

class Game(models.Model):
    name = models.TextField(blank=False, null=False, max_length=255, default='')
    price = models.DecimalField(max_digits=10, decimal_places=2, null=False, default=0)
    url = models.URLField(max_length=200, blank=False, default='')
    developer =  models.ForeignKey('users.CustomUser', null=False, on_delete=models.CASCADE)

class Highscores(models.Model):
    game = models.ForeignKey('Game', on_delete=models.CASCADE, null=False)
    username = models.TextField(blank=False, null=False, max_length=255, default='')
    score = models.DecimalField(max_digits=100, decimal_places=2, default=0)

class Purchases(models.Model):
    user = models.ForeignKey('users.CustomUser', on_delete=models.CASCADE, null=False)
    price = models.DecimalField(max_digits=10, decimal_places=2, null=False, default=0)
    game = models.ForeignKey('Game', on_delete=models.CASCADE, null=False)
    timestamp = models.DateTimeField(auto_now_add=True, null=False)
