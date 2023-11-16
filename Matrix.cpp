#include "Matrix.h"


Matrix::Matrix(unsigned int nrRows, unsigned int nrCols) : row_count(nrRows), column_count(nrCols) {
    if(nrRows<=0 || nrCols <= 0)
        throw std::runtime_error{"Cannot have negative or null matrix dimensions"};
    element_count = 0;
    table_size = 4;
    while (table_size < nrRows || table_size < nrCols)
        table_size <<= 1;
    element_list = new TElem [table_size] {NULL_TELEM};
    row_list = new int [table_size] {-1 };
    column_list = new int [table_size] {-1 };
    number_of_deleted_elements = 0;
}

Matrix::~Matrix() {
    delete[] element_list;
    delete[] column_list;
    delete[] row_list;
}

unsigned int Matrix::nrLines() const { return row_count; }


unsigned int Matrix::nrColumns() const { return column_count; }

unsigned int Matrix::hash(unsigned int row, unsigned int col) const { return (((row ^ col) << 1) + ((row & col) >> 1)) % table_size; }

TElem Matrix::element(unsigned int i, unsigned int j) const {
	if (i >= row_count )
		throw std::runtime_error("Invalid row id");
	if (j >= column_count )
		throw std::runtime_error("Invalid col id");
	unsigned int position = hash(i,j);
    unsigned int attempt = 0;
    while(!((i == row_list[position] && j == column_list[position]) || (element_list[position] == NULL_TELEM) || (attempt>= table_size))){
		/// while(not(position_match) and not(position_empty))
		attempt ++;
		position = (position + (attempt >> 1) + ((attempt * attempt)>>1)) % table_size;
	}
    return element_list[position] == DELETED_TELEM ? NULL_TELEM : element_list[position];
}

TElem Matrix::modify(unsigned int i, unsigned int j, TElem e) {
	if (i >= row_count )
		throw std::runtime_error("Invalid row id");
	if (j >= column_count )
		throw std::runtime_error("Invalid col id");
	unsigned int position = hash(i,j);
	unsigned int attempt = 0;
    while(!((i == row_list[position] && j == column_list[position]) || (element_list[position] == NULL_TELEM) || (attempt>= table_size))){
		/// while(not((position_match) or (position_available)))
		attempt ++;
        position = (position + (attempt >> 1) + ((attempt * attempt)>>1)) % table_size;
	}
	if(e == NULL_TELEM){ /// If we have to delete the element at whichever position we discovered
		if(element_list[position] == NULL_TELEM || element_list[position] == DELETED_TELEM){ /// If there is no element to delete
			return NULL_TELEM;
		}
		auto old = element_list[position];
        element_list[position] = DELETED_TELEM;/// Deletion takes the form of an overriding with del value
		number_of_deleted_elements ++;
		element_count --;
		if (1.0 * number_of_deleted_elements / table_size > deleted_elements_ratio_threshold ) /// >:(((((((((((((((((((((
            rehash();
		return old;
	}
	if(element_list[position] == NULL_TELEM || element_list [position] == DELETED_TELEM){ /// If an empty position was discovered
        row_list[position] = (int)i;
        column_list[position] = (int)j;
        element_list[position] = e;
        element_count++;
        number_of_deleted_elements += (element_list[position] == DELETED_TELEM) ? -1 : 0;
        if (1.0 * element_count / table_size >= load_ratio_threshold) {
            resizeAndRehash();
        }
        return e;
	}else{ /// we have to override an existing value
		auto old = element_list[position];
        element_list[position] = e;
		return old;
	}
}

void Matrix::rehash(){
    auto newRows = new int [ table_size ] {NULL_TELEM };
    auto newCols = new int [ table_size ] {NULL_TELEM };
    auto newElems = new TElem [ table_size ] {NULL_TELEM };
    unsigned int position, attempt;
    for(int i = 0 ; i < table_size ; i ++){
        if(element_list[i] != NULL_TELEM && element_list[i] != DELETED_TELEM ){
            attempt = 0;
            position = hash(row_list[i], column_list[i]);
            while(newElems[position] !=  NULL_TELEM && (attempt < table_size)){
                attempt ++;
                position = (position + (attempt >> 1) + ((attempt * attempt)>>1)) % table_size;
            }
            newElems[position] = element_list[i];
            newRows[position] = row_list[i];
            newCols[position] = column_list[i];
        }
    }
    number_of_deleted_elements = 0;
    delete[]element_list; element_list = newElems;
    delete[]column_list; column_list = newCols;
    delete[]row_list; row_list = newRows;
}

void Matrix::resizeAndRehash(){
    table_size <<= 1;
    auto newRows = new int [ table_size ] {NULL_TELEM };
    auto newCols = new int [ table_size ] {NULL_TELEM };
    auto newElems = new TElem [ table_size ] {NULL_TELEM };
    unsigned int position, attempt;
	for(int i = 0 ; i < ( table_size >> 1 ) ; i ++){
		if(element_list[i] != NULL_TELEM && element_list[i] != DELETED_TELEM){
            attempt = 0;
            position = hash(row_list[i], column_list[i]);
            while(newElems[position] !=  NULL_TELEM && attempt < table_size){
                attempt ++;
                position = (position + (attempt >> 1) + ((attempt * attempt)>>1)) % table_size;
            }
            newElems[position] = element_list[i];
            newRows[position] = row_list[i];
            newCols[position] = column_list[i];
		}
	}
    number_of_deleted_elements = 0;
    delete[]element_list; element_list = newElems;
    delete[]column_list; column_list = newCols;
    delete[]row_list; row_list = newRows;
}

void Matrix::setMainDiagonal(TElem elem) {
    if(row_count != column_count)
        throw std::runtime_error{"Matrix is not square :-("};
    for(int i = 0; i < row_count; i ++){
        modify(i,i,elem);
    }
}
