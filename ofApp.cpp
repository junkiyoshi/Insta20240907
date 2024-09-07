#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofSetColor(239);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	float span = 1;
	float threshold_1 = 0.49;
	float threshold_2 = 0.51;
	float frame_param = ofGetFrameNum() * 0.005;
	float coordinate_param = 0.008;
	float len = 400;
	float angle;
	glm::highp_mat4 rotation;

	ofColor color;

	for (float z = 0; z >= -len * 0.25; z -= 6) {

		color = ofColor(ofMap(z, -len * 0.25, 0, 239, 39));

		threshold_1 -= 0.01;
		threshold_2 += 0.01;

		for (float x = -len; x <= len; x += span) {

			for (float y = -len; y <= len; y += span) {

				glm::vec3 noise_location = glm::vec4(x, y, z, 0) * rotation;
				auto noise_value = ofNoise(glm::vec4(noise_location * coordinate_param, frame_param));
				if (noise_value < threshold_1 || noise_value > threshold_2) { continue; }

				glm::vec3 loc_1 = glm::vec4(x - span, y, z, 0) * rotation;
				auto noise_1 = ofNoise(glm::vec4(loc_1 * coordinate_param, frame_param));
				glm::vec3 loc_2 = glm::vec4(x, y + span, z, 0) * rotation;
				auto noise_2 = ofNoise(glm::vec4(loc_2 * coordinate_param, frame_param));
				glm::vec3 loc_3 = glm::vec4(x + span, y, z, 0) * rotation;
				auto noise_3 = ofNoise(glm::vec4(loc_3 * coordinate_param, frame_param));
				glm::vec3 loc4 = glm::vec4(x, y - span, z, 0) * rotation;
				auto noise_4 = ofNoise(glm::vec4(loc4 * coordinate_param, frame_param));

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(glm::vec4(x - span * 0.5, y - span * 0.5, z, 0) * rotation);
				vertices.push_back(glm::vec4(x + span * 0.5, y - span * 0.5, z, 0) * rotation);
				vertices.push_back(glm::vec4(x + span * 0.5, y + span * 0.5, z, 0) * rotation);
				vertices.push_back(glm::vec4(x - span * 0.5, y + span * 0.5, z, 0) * rotation);

				this->face.addVertices(vertices);

				for (int i = 0; i < 4; i++) { this->face.addColor(color); }

				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 2);
				this->face.addIndex(index + 0); this->face.addIndex(index + 2); this->face.addIndex(index + 3);

				if (noise_1 < threshold_1 || noise_1 > threshold_2 || x == -len) {

					this->frame.addVertex(vertices[0]);
					this->frame.addVertex(vertices[3]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_2 < threshold_1 || noise_2 > threshold_2 || y == len) {

					this->frame.addVertex(vertices[2]);
					this->frame.addVertex(vertices[3]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_3 < threshold_1 || noise_3 > threshold_2 || x == len) {

					this->frame.addVertex(vertices[1]);
					this->frame.addVertex(vertices[2]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_4 < threshold_1 || noise_4 > threshold_2 || y == -len) {

					this->frame.addVertex(vertices[0]);
					this->frame.addVertex(vertices[1]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}
			}

		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	this->frame.draw();
	this->face.draw();

	/*
	int start = 1;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}