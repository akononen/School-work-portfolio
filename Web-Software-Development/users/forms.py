from django import forms
from django.contrib.auth.forms import UserCreationForm, UserChangeForm
from users.models import CustomUser

class CustomUserCreationForm(UserCreationForm):
    def __init__(self, *args, **kwargs):
        super(CustomUserCreationForm, self).__init__(*args, **kwargs)
        self.fields['is_developer'].help_text = "(Leave unchecked to register as player)"
        self.fields['is_developer'].label = "I am Developer"
    class Meta:
        model = CustomUser
        fields = ['username', 'email', 'is_developer']

class CustomUserChangeForm(UserChangeForm):

    class Meta:
        model = CustomUser
        fields = UserChangeForm.Meta.fields
