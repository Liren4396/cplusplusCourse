#ifndef COMP6771_MATRIX_H
#define COMP6771_MATRIX_H

class matrix {
public:
    matrix() noexcept = default;
    matrix(std::initializer_list<std::initializer_list<int>> il);

    matrix(const matrix &other);
    matrix(matrix &&other);
private:
    std::unique_ptr<int[]> data_;
    std::size_t n_rows_;
    std::size_t n_cols_;
};

#endif // COMP6771_MATRIX_H
