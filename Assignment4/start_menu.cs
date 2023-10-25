using Godot;
using System;

public partial class start_menu : Node2D
{
	private void _on_start_pressed()
	{
		GetTree().ChangeSceneToFile("res://scenes/main.tscn");
	}
	
	private void _on_exit_pressed()
	{
		GetTree().Quit();
	}
}
