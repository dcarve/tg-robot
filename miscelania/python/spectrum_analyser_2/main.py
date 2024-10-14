import time
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.ticker as ticker
import json
import configs
import calcs


# {"millis":78361,"rpm1":123.09,"filterRpm_1":123.08,"rpm2":140.68,"filterRpm_2":125.58,"rpm3":-105.51,"filterRpm_3":-114.47}
class AnimationPlot:
    def animate(
        self,
        i,
        rpm1_list,
        rpm2_list,
        rpm3_list,
        filter_rpm_1_list,
        filter_rpm_2_list,
        filter_rpm_3_list,
        w1_list,
        w2_list,
        w3_list,
        seconds_list,
        ser,
    ):
        decoded_bytes = ser.readline().decode("utf-8").strip("\r\n")
        
        try:
            dict_data = json.loads(decoded_bytes)

            rpm1_list.append(abs(dict_data["rpm1"]))
            rpm2_list.append(abs(dict_data["rpm2"]))
            rpm3_list.append(abs(dict_data["rpm3"]))

            filter_rpm_1_list.append(abs(dict_data["filterRpm_1"]))
            filter_rpm_2_list.append(abs(dict_data["filterRpm_2"]))
            filter_rpm_3_list.append(abs(dict_data["filterRpm_3"]))
                        
            w1_list.append(abs(dict_data["w1"]))
            w2_list.append(abs(dict_data["w2"]))
            w3_list.append(abs(dict_data["w3"]))

            seconds_list.append(calcs.millis_to_seconds(dict_data["millis"]))

        except:
            pass
    

        rpm1_list = rpm1_list[-configs.CHUNK_DATA :]
        rpm2_list = rpm2_list[-configs.CHUNK_DATA :]
        rpm3_list = rpm3_list[-configs.CHUNK_DATA :]

        filter_rpm_1_list = filter_rpm_1_list[-configs.CHUNK_DATA :]
        filter_rpm_2_list = filter_rpm_2_list[-configs.CHUNK_DATA :]
        filter_rpm_3_list = filter_rpm_3_list[-configs.CHUNK_DATA :]

        w1_list = w1_list[-configs.CHUNK_DATA :]
        w2_list = w2_list[-configs.CHUNK_DATA :]
        w3_list = w3_list[-configs.CHUNK_DATA :]

        seconds_list = seconds_list[-configs.CHUNK_DATA :]
        
        ax11.clear()
        ax21.clear()
        ax21_2.clear()
        
        ax12.clear()
        ax22.clear()
        ax22_2.clear()
        
        ax13.clear()
        ax23.clear()
        ax23_2.clear()

        self.plot_format()

        ax11.plot(
            seconds_list, rpm1_list, color="#3684eb", label="rpm 1"
        )
        ax21.plot(
            seconds_list, filter_rpm_1_list, color="#3684eb", label="rpm filtered 1"
        )
        ax21_2.plot(
            seconds_list, w1_list, color="#ff9d00", label="pwm 1"
        )
        

        ax12.plot(
            seconds_list, rpm2_list, color="#3684eb", label="rpm 2"
        )
        ax22.plot(
            seconds_list, filter_rpm_2_list, color="#3684eb", label="rpm filtered 2"
        )
        ax22_2.plot(
            seconds_list, w2_list, color="#ff9d00", label="pwm 2"
        )


        ax13.plot(
            seconds_list, rpm3_list, color="#3684eb", label="rpm 3"
        )
        ax23.plot(
            seconds_list, filter_rpm_3_list, color="#3684eb", label="rpm filtered 3"
        )
        ax23_2.plot(
            seconds_list, w3_list, color="#ff9d00", label="pwm 3"
        )


        ax11.legend()
        ax21.legend()
        ax21_2.legend()

        ax12.legend()
        ax22.legend()
        ax22_2.legend()

        ax13.legend()
        ax23.legend()
        ax23_2.legend()

    def plot_format(self):

        ax11.set_ylim([0, 220])
        ax11.set_title("rpm 1")
        ax11.set_xlabel("secconds")
        ax11.set_ylabel("rpm")
        
        ax12.set_ylim([0, 220])
        ax12.set_title("rpm 2")
        ax12.set_xlabel("secconds")
        ax12.set_ylabel("rpm")
        
        ax13.set_ylim([0, 220])
        ax13.set_title("rpm 3")
        ax13.set_xlabel("secconds")
        ax13.set_ylabel("rpm")


        ax21.set_ylim([0, 220])
        ax21.set_title("RPM x PWM 1")
        ax21.set_xlabel("secconds")
        ax21.set_ylabel("rpm")
        
        ax21_2.set_ylim([0, 65540])
        ax21_2.set_ylabel("pwm")

        ax22.set_ylim([0, 220])
        ax22.set_title("RPM x PWM 2")
        ax22.set_xlabel("secconds")
        ax22.set_ylabel("rpm")
        
        ax22_2.set_ylim([0, 65540])
        ax22_2.set_ylabel("pwm")

        ax23.set_ylim([0, 220])
        ax23.set_title("RPM x PWM 3")
        ax23.set_xlabel("secconds")
        ax23.set_ylabel("rpm")
        
        ax23_2.set_ylim([0, 65540])
        ax23_2.set_ylabel("pwm")




rpm1_list = list()
rpm2_list = list()
rpm3_list = list()

filter_rpm_1_list = list()
filter_rpm_2_list = list()
filter_rpm_3_list = list()

w1_list = list()
w2_list = list()
w3_list = list()

seconds_list = list()

my_dpi=100
fig = plt.figure(figsize=(2626/my_dpi, 911/my_dpi), dpi=my_dpi)



ax11 = fig.add_subplot(2, 3, 1)
ax12 = fig.add_subplot(2, 3, 2)
ax13 = fig.add_subplot(2, 3, 3)

ax21 = fig.add_subplot(2, 3, 4)
ax21_2 = ax21.twinx()

ax22 = fig.add_subplot(2, 3, 5)
ax22_2 = ax22.twinx()

ax23 = fig.add_subplot(2, 3, 6)
ax23_2 = ax23.twinx()


realTimePlot = AnimationPlot()

ser = serial.Serial(configs.PORT, configs.BAUDRATE)
ser.setDTR(False)
time.sleep(1)
ser.flushInput()
ser.setDTR(True)


ani = animation.FuncAnimation(
    fig,
    realTimePlot.animate,
    frames=configs.CHUNK_DATA,
    fargs=(
        rpm1_list,
        rpm2_list,
        rpm3_list,
        filter_rpm_1_list,
        filter_rpm_2_list,
        filter_rpm_3_list,
        w1_list,
        w2_list,
        w3_list,
        seconds_list,
        ser,
    ),
    interval=10,
)

plt.show()
