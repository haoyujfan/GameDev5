extends Control

func _on_singleplayer_pressed():
	pass # Replace with function body.


func _on_multiplayer_pressed():
	get_tree().change_scene_to_file("res://scenes/start_menu.tscn")
	pass # Replace with function body.
