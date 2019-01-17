from django import forms
from django.contrib.auth.forms import UserCreationForm
from  django.forms import HiddenInput

from WebStore.models import Game

class AddGameForm(forms.ModelForm):
    #name = forms.CharField(required=True, max_length=80)

    # We redefine price so we can define the maximum number of decimals
    #price = forms.DecimalField(required=True, decimal_places=2, max_digits=9)
    def __init__(self, *args, **kwargs):
        super(AddGameForm, self).__init__(*args, **kwargs)
        self.fields['name'].required = True
        self.fields['price'].required = True
        self.fields['url'].required = True
        #self.fields['developer'].widget = HiddenInput()
        #self.fields['developer'].initial = 5


    class Meta:
        model = Game
        #dields = ('name', 'price', 'url', 'developer')
        fields = ('name', 'price', 'url')
