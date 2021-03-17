import pygame
import socket
pygame.init()
done = False
clock = pygame.time.Clock()
pygame.joystick.init()
import mysql.connector


try:
    host = '192.168.1.13'  # as both code is running on same pc
    port = 23   # socket server port number
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket = socket.socket()  # instantiate
    client_socket.connect((host, port))  # connect to the server
    connection = True
except socket.timeout:
    print("Connection ERROR")

#def upload_to_mysql(vay):
#    cnx = mysql.connector.connect(host='127.0.0.1', user='root', database='thetha')
#    cursor = cnx.cursor()
#
#    sql = "UPDATE dir SET direction = %s WHERE id = %s"
#    val = (vay, 1)
#    cursor.execute(sql, val)
#    # Make sure data is committed to the database
#    cnx.commit()
#
#    cursor.close()
#    cnx.close()




while not done:
    def send(command):
        if (connection == True):
            message = command
            client_socket.send(message.encode())  # send message


    for event in pygame.event.get(): # User did something.
        if event.type == pygame.QUIT: # If user clicked close.
            done = True # Flag that we are done so we exit this loop.
        elif event.type == pygame.JOYBUTTONDOWN:
            print("Joystick button pressed.")
        elif event.type == pygame.JOYBUTTONUP:
            print("Joystick button released.")
    joystick_count = pygame.joystick.get_count()
    for i in range(joystick_count):
        joystick = pygame.joystick.Joystick(i)
        joystick.init()
        try:
            jid = joystick.get_instance_id()
        except AttributeError:
            jid = joystick.get_id()
        name = joystick.get_name()
        axes = joystick.get_numaxes()
        axis1 = joystick.get_axis(1)
        axis2 = joystick.get_axis(2)
        button1 = joystick.get_button(5)
        button2 = joystick.get_button(4)
        button3 = joystick.get_button(3)
        button4 = joystick.get_button(0)

        if (button1 == 1):
            print("right")
            send("back_direction_right")
        if (button2 == 1):
            print("left")
            send("back_direction_left")
        if (button3 == 1):
            print("speed_up")
            send("motor_speed_up")
        if (button4 == 1):
            print("speed_down")
            send("motor_speed_down")
        print(axis1)
        if (axis1 <= -0.900):
            send("direction_front")
        if (axis1 >= 0.900):
            send("direction_back")
        if (axis1 < 0.900 and axis1 > -0.900):
            send("direction_stay")
        print(axis2)
        if (axis2 <= -0.900):
            send("front_direction_left")
        if (axis2 >= 0.900):
            send("front_direction_right")
pygame.quit()
