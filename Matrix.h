#pragma once
#include <stdexcept>
#include <cmath>
/* ADT Matrix–
 * represented as a sparse matrix where <line, column, value> triples (value ≠0) are memorized,
 * ordered lexicographically considering the line and column of every element.
 * The elements are stored in a hashtable with open addressing, quadratic probing.
 * */

//DO NOT CHANGE THIS PART
typedef int TElem;
#define NULL_TELEM 0
#define DELETED_TELEM INT_MIN
class Matrix {

private:
    unsigned int row_count;
    unsigned int column_count;
	unsigned int element_count;
	unsigned int table_size;
	int* row_list;
	int* column_list;
	TElem* element_list;
    unsigned int number_of_deleted_elements;
	constexpr static double const load_ratio_threshold{0.75};
    constexpr static double const deleted_elements_ratio_threshold{0.33};

    [[nodiscard]] unsigned int hash(unsigned int row, unsigned int col)const;
    /// \Time: Theta(1)
    /// \Space: Theta(1)
    /// \params: row and col -> unsigned (positive) integers
    /// \return: hashed value of position (row,column) inside the matrix
    /// \errors: throws no custom errors

	void resizeAndRehash();
    /// \Time: O(n)
    /// \Space: Theta(2 * table_size)
    /// \errors: throws no custom errors

    void rehash();
    /// \Time: O(n)
    /// \Space: Theta(table_size)
    /// \errors: throws no custom errors

public:
	//constructor
    Matrix(unsigned int nrRows, unsigned int nrCols);
    /// \time: O(log2(max(nrRows,nrCols))
    /// \space: O(max(nrRows, nrCols))
    /// \params: nrRows- unsigned integer representing the number of row_count of the matrix; nrCols- unsigned integer representing the number of columns of the matrix
    /// \errors: throws runtime error if nrRows or nrCols is less than or equal to 0



    ~Matrix();

	//returns the number of lines
	[[nodiscard]] unsigned int nrLines() const;
    /// \Time: Theta(1)
    /// \Space: Theta(1)
    /// \params: None
    /// \return: number of row_count of the matrix
    /// \errors: throws no custom errors

	//returns the number of columns
	[[nodiscard]] unsigned int nrColumns() const;
    /// \Time: Theta(1)
    /// \Space: Theta(1)
    /// \params: None
    /// \return: number of columns of the matrix
    /// \errors: throws no custom errors

	//returns the element from line i and column j (indexing starts from 0)
	//throws exception if (i,j) is not a valid position in the Matrix
	TElem element(unsigned int i, unsigned int j) const; // NOLINT(modernize-use-nodiscard)
    /// \Time: O(1)
    /// \Space: Theta(1)
    /// \params: i- unsigned integer representing the row index; j- unsigned integer representing the column index
    /// \return: value at position (i,j) inside the matrix
    /// \errors: throws runtime error if the indices do not fit into the matrix bounds

	//modifies the value from line i and column j
	//returns the previous value from the position
	//throws exception if (i,j) is not a valid position in the Matrix
	TElem modify(unsigned int i, unsigned int j, TElem e);
    /// \Time: O(1) - in think ???
    /// \Space: O(1)
    /// \params: i- unsigned integer representing the row index; j- unsigned integer representing the column index, e- TElem which will replace value at position (i,j)
    /// \return: either the overwritten element (in the case of deletion) or the newly inserted element (in the case when previously (i,j) had NULL_TELEM
    /// \errors: throws runtime error if the indices do not fit into the matrix bounds

    void setMainDiagonal(TElem elem);
    /// \Time: Theta(nrRows)
    /// \Space: Theta(1)
    /// \params: elem -> TElem representing the value to be set on the main diagonal
    /// \errors: throws runtime error if the matrix is not square (ie does not have a well-defined main diagonal
};
