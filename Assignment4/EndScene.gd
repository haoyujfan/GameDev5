extends Node2D

# restart button
func _on_restart_pressed():
	if GameManager.mode == "multiplayer":
		get_tree().change_scene_to_file("res://scenes/start_menu.tscn")
	else:
		get_tree().change_scene_to_file("res://scenes/main_menu.tscn")

# quit button
func _on_quit_pressed():
	get_tree().quit()
