#include "gif.h"
#include <filesystem> 
#include "global_variables.h"

gif::gif(float x, float y, std::string folder_name, float delay_time)
	: m_x1(x),
	  m_y1(y),
	  m_folder_name(folder_name),
	  m_index(0),
	  m_delay_time(delay_time)
{
	int i = 0;
	for (const auto& entry : std::filesystem::directory_iterator(PATH + "\\images\\" + folder_name)) i++;

	m_length = i;
	m_images = new image[i];

	i = 0;
	for (const auto& entry : std::filesystem::directory_iterator(PATH + "\\images\\" + folder_name)) {
		std::string d = entry.path().string();
		for (int k = 4; k < d.size(); k++) {
			if (d[d.size() - k] == '\\') {
				m_images[i].init(x, y, d.substr(d.size() - k + 1, k));
				break;
			}
		}
		//println(entry.path().string());
		//m_images[i].init(x, y, "", entry.path().string());
		i++;
	}
	m_x2 = m_x1 + m_images[0].get_x2() - m_images[0].get_x();
	m_y2 = m_y1 + m_images[0].get_y2() - m_images[0].get_y();


}
void gif::draw() {
	m_images[m_index].draw();

	m_total_time += delta_time;
	if (m_total_time >= m_delay_time) {
		m_total_time -= m_delay_time;

		m_index++;
		if (m_index == m_length) m_index = 0;
	}
}
gif::~gif() {
	delete[] m_images;
}

void gif::move(float x, float y) {
}
void gif::set_position(float x1, float y1, float x2, float y2) {

}