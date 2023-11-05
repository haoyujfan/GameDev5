extends Control

func _on_singleplayer_pressed():
	GameManager.Players[1] = { 
	"name": "default",
	"id" : 1,
	"score" : 0
	}
	GameManager.mode = "singleplayer"
	get_tree().change_scene_to_file("res://scenes/main.tscn")


func _on_multiplayer_pressed():
	GameManager.mode = "multiplayer"
	get_tree().change_scene_to_file("res://scenes/start_menu.tscn")

