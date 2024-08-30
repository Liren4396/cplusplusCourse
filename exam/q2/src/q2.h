#ifndef Q2_HPP_INCLUDED
#define Q2_HPP_INCLUDED

#include <array>
#include <cstddef>
#include <string>
#include <iostream>
namespace q2 {
	
	enum class Move { up, down, left, right };
	using cell = std::string;
	template <std::size_t size>
	std::string merge_cell(std::array<std::array<q2::cell, size>, size>& grid, std::string curr_cell, int row, int col) {
		if (grid[row][col] == "0x1") {
			grid[row][col] = "0x2";
		} else if (grid[row][col] == "0x2") {
			grid[row][col] = "0x4";
		} else if (grid[row][col] == "0x4") {
			grid[row][col] = "0x8";
		} else if (grid[row][col] == "0x8") {
			grid[row][col] = "0x10";
		} else if (grid[row][col] == "0x10") {
			grid[row][col] = "0x20";
		} else if (grid[row][col] == "0x20") {
			grid[row][col] = "0x40";
		} else if (grid[row][col] == "0x40") {
			grid[row][col] = "0x80";
		} else if (grid[row][col] == "0x80") {
			grid[row][col] = "0x100";
		} else if (grid[row][col] == "0x100") {
			grid[row][col] = "0x200";
		} else if (grid[row][col] == "0x200") {
			grid[row][col] = "0x400";
		} else if (grid[row][col] == "0x400") {
			grid[row][col] = "0x800";
			std::cout << "game over you win" << std::endl;
		}
		return grid[row][col];
	}
	template <std::size_t size>
	void up_move(std::array<std::array<q2::cell, size>, size>& grid, int n) {
		
	}
	
	template <std::size_t size>
	void move_0x800(std::array<std::array<q2::cell, size>, size>& grid, q2::Move direction) {
		int n = grid.size();
		if (direction == Move::up) {
			for (int j = 0; j < n; ++j) {
				for (int i = 0; i < n; ++i) {
					int z = i-1;
					
					if (grid[z][j] == "") continue;
					std::string curr_cell = grid[z][j];
					while (z > 0 && grid[z][j] != "") --z;
					if (grid[z][j] == curr_cell) {
						std::string merge_ret = merge_cell(grid, curr_cell, z, j);
						grid[z][j] = merge_ret;
					} else if (z == 0 && grid[z][j] == "") {
						grid[z][j] == grid[i][j];
					} else {
						grid[z+1][j] = curr_cell;
					}
					grid[i][j] = "";
				}
			}
		} else if (direction == Move::down) {
			for (int j = n-1; j >= 0; --j) {
				for (int i = n-1; i >= 0; --i) {
					int z = i+1;
					
					if (grid[i][j] == "") continue;
					std::string curr_cell = grid[i][j];
					while (z < n && grid[z][j] == "") ++z;
					if (grid[z][j] == curr_cell) {
						std::string merge_ret = merge_cell(grid, curr_cell, z, j);
						grid[z][j] = merge_ret;
					} else if (z == n-1 && grid[z][j] == "") {
						grid[z][j] == grid[i][j];
					} else {
						grid[z-1][j] = curr_cell;
					}
					grid[i][j] = "";
				}
			}
		} else if (direction == Move::right) {
			for (int i = n-1; i >= 0; --i) {
				for (int j = n-1; j >= 0; --j) {
					int z = j+1;
					
					if (grid[i][z] == "") continue;
					std::string curr_cell = grid[i][j];
					while (z < n && grid[i][z] == "") ++z;
					if (grid[i][z] == curr_cell) {
						std::string merge_ret = merge_cell(grid, curr_cell, z, j);
						grid[i][z] = merge_ret;
					} else if (z == n-1 && grid[i][z] == "") {
						grid[i][z] == grid[i][j];
					} else {
						grid[i][z-1] = curr_cell;
					}
					grid[i][j] = "";
				}
			}
		} else if (direction == Move::left) {
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					int z = j-1;
					
					if (grid[i][z] == "") continue;
					std::string curr_cell = grid[z][j];
					while (z > 0 && grid[i][z] != "") --z;
					if (grid[i][z] == curr_cell) {
						std::string merge_ret = merge_cell(grid, curr_cell, z, j);
						
						grid[i][z] = merge_ret;
					} else if (z == 0 && grid[i][z] == "") {
						grid[i][z] == grid[i][j];
					} else {
						grid[i][z+1] = curr_cell;
					}
					grid[i][j] = "";
				}
			}
		}
	}

} // namespace q2

#endif
