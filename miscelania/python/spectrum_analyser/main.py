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

            seconds_list.append(calcs.millis_to_seconds(dict_data["millis"]))

        except:
            pass
    

        rpm1_list = rpm1_list[-configs.CHUNK_DATA :]
        rpm2_list = rpm2_list[-configs.CHUNK_DATA :]
        rpm3_list = rpm3_list[-configs.CHUNK_DATA :]

        filter_rpm_1_list = filter_rpm_1_list[-configs.CHUNK_DATA :]
        filter_rpm_2_list = filter_rpm_2_list[-configs.CHUNK_DATA :]
        filter_rpm_3_list = filter_rpm_3_list[-configs.CHUNK_DATA :]

        seconds_list = seconds_list[-configs.CHUNK_DATA :]
        
        for name_ax in list_ax:
            globals()[name_ax].clear()

        fcycles_values = calcs.fcycles(seconds_list)
        fft_values_1 = calcs.fft_values(rpm1_list)
        fft_values_2 = calcs.fft_values(rpm2_list)
        fft_values_3 = calcs.fft_values(rpm3_list)

        rpm_1_list_low_pass = calcs.low_pass_filter_rpm_1(rpm1_list)
        rpm_2_list_low_pass = calcs.low_pass_filter_rpm_2(rpm2_list)
        rpm_3_list_low_pass = calcs.low_pass_filter_rpm_2(rpm3_list)

        fft_low_pass_values_1 = calcs.fft_values(rpm_1_list_low_pass)
        fft_low_pass_values_2 = calcs.fft_values(rpm_2_list_low_pass)
        fft_low_pass_values_3 = calcs.fft_values(rpm_3_list_low_pass)

        self.plot_format()

        globals()[motor1_plot[0]].plot(
            seconds_list, rpm1_list, color="#3684eb", label="rpm 1"
        )
        globals()[motor1_plot[1]].plot(
            fcycles_values, fft_values_1, color="#3684eb", label="freq 1"
        )
        globals()[motor1_plot[2]].plot(
            seconds_list, rpm_1_list_low_pass, color="#3684eb", label="rpm 1"
        )
        globals()[motor1_plot[3]].plot(
            fcycles_values, fft_low_pass_values_1, color="#3684eb", label="freq 1"
        )
        #globals()[motor1_plot[4]].plot(
        #    seconds_list, filter_rpm_1_list, color="#3684eb", label="rpm filtrado real 1"
        #)

        globals()[motor2_plot[0]].plot(
            seconds_list, rpm2_list, color="#3684eb", label="rpm 2"
        )
        globals()[motor2_plot[1]].plot(
            fcycles_values, fft_values_2, color="#3684eb", label="freq 2"
        )
        globals()[motor2_plot[2]].plot(
            seconds_list, rpm_2_list_low_pass, color="#3684eb", label="rpm 2"
        )
        globals()[motor2_plot[3]].plot(
            fcycles_values, fft_low_pass_values_2, color="#3684eb", label="freq 2"
        )
        #globals()[motor2_plot[4]].plot(
        #    seconds_list, filter_rpm_2_list, color="#3684eb", label="rpm filtrado real 2"
        #)

        globals()[motor3_plot[0]].plot(
            seconds_list, rpm3_list, color="#3684eb", label="rpm 3"
        )
        globals()[motor3_plot[1]].plot(
            fcycles_values, fft_values_3, color="#3684eb", label="freq 3"
        )
        globals()[motor3_plot[2]].plot(
            seconds_list, rpm_3_list_low_pass, color="#3684eb", label="rpm 3"
        )
        globals()[motor3_plot[3]].plot(
            fcycles_values, fft_low_pass_values_3, color="#3684eb", label="freq 3"
        )
        #globals()[motor3_plot[4]].plot(
        #    seconds_list, filter_rpm_3_list, color="#3684eb", label="rpm filtrado real 3"
        #)

        for name_ax in list_ax:
            if name_ax not in ["ax_2_6_14", "ax_2_6_16", "ax_2_6_18"]:
                globals()[name_ax].legend()

    def plot_format(self):

        for idx, ax in enumerate([motor1_plot[0], motor2_plot[0], motor3_plot[0]]):
            globals()[ax].set_ylim([0, 200])
            globals()[ax].set_title("rpm_" + str(idx + 1))
            globals()[ax].set_xlabel("secconds")
            globals()[ax].set_ylabel("rpm")

        for idx, ax in enumerate([motor1_plot[1], motor2_plot[1], motor3_plot[1]]):
            globals()[ax].set_xlim([0, 30])
            globals()[ax].set_ylim([10 ** (-1), 10 ** 5])
            globals()[ax].set_yscale("symlog")
            globals()[ax].set_title("freq rpm_" + str(idx + 1))
            globals()[ax].set_xlabel("Hz")

        for idx, ax in enumerate([motor1_plot[2], motor2_plot[2], motor3_plot[2]]):
            globals()[ax].set_ylim([0, 200])
            globals()[ax].set_title("passa baixa rpm_" + str(idx + 1))
            globals()[ax].set_xlabel("secconds")
            globals()[ax].set_ylabel("rpm")

        for idx, ax in enumerate([motor1_plot[3], motor2_plot[3], motor3_plot[3]]):
            globals()[ax].set_xlim([0, 30])
            globals()[ax].set_ylim([10 ** (-1), 10 ** 5])
            globals()[ax].set_yscale("symlog")
            globals()[ax].set_title("passa baixa freq rpm_" + str(idx + 1))
            globals()[ax].set_xlabel("Hz")

        #for idx, ax in enumerate([motor1_plot[4], motor2_plot[4], motor3_plot[4]]):
        #    globals()[ax].set_ylim([0, 200])
        #    globals()[ax].set_title("rpm filtrado real " + str(idx + 1))
        #    globals()[ax].set_xlabel("secconds")
        #    globals()[ax].set_ylabel("rpm")



rpm1_list = list()
rpm2_list = list()
rpm3_list = list()

filter_rpm_1_list = list()
filter_rpm_2_list = list()
filter_rpm_3_list = list()

seconds_list = list()

my_dpi=100
fig = plt.figure(figsize=(2626/my_dpi, 911/my_dpi), dpi=my_dpi)

motor1_plot = ["ax_2_6_01", "ax_2_6_02"] + ["ax_2_6_07", "ax_2_6_08"]# + ["ax_2_6_13", "ax_2_6_14"]
motor2_plot = ["ax_2_6_03", "ax_2_6_04"] + ["ax_2_6_09", "ax_2_6_10"]# + ["ax_2_6_15", "ax_2_6_16"]
motor3_plot = ["ax_2_6_05", "ax_2_6_06"] + ["ax_2_6_11", "ax_2_6_12"]# + ["ax_2_6_17", "ax_2_6_18"]

list_ax = sorted(motor1_plot + motor2_plot + motor3_plot)

for idx, name_ax in enumerate(list_ax):
    variable_value = fig.add_subplot(2, 6, idx + 1)
    #variable_value = fig.add_subplot(3, 6, idx + 1)
    globals()[name_ax] = variable_value


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
        seconds_list,
        ser,
    ),
    interval=10,
)

plt.show()
