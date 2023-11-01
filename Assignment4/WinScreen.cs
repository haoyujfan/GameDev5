using Godot;
using System;

public partial class WinScreen : Node2D
{
	private Label label;
	// restart button
	private void _on_restart_pressed()
	{
		GetTree().ChangeSceneToFile("res://scenes/main.tscn");
	}
	
	// quit button
	private void _on_quit_pressed()
	{
		GetTree().ChangeSceneToFile("res://scenes/main_menu.tscn");
	}

}





