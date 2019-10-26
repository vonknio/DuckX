/*
 * Under MIT license
 * Author: Amir Mohamadi (@amiremohamadi)
 * DuckX is a free library to work with docx files.
*/

#ifndef DUCKX_H
#define DUCKX_H

#include <stdlib.h>
#include <cstdio>
#include <string>

#include "pugixml.hpp"
#include "zip.h"


// TODO: Use container-iterator design pattern!

namespace duckx {
	template<typename T>
	class Iterator;
    // Run contains runs in a paragraph
    class Run {
    private:
        friend class Iterator<Run>;
        // Store the parent node (a paragraph)
        pugi::xml_node parent;
        // And store current node also
        pugi::xml_node current;

    public:
        Run();
        Run(pugi::xml_node, pugi::xml_node);
        void set_parent(pugi::xml_node);
        void set_current(pugi::xml_node);

        std::string get_text() const;
        bool set_text(const std::string&) const;
        bool set_text(const char *) const;

        Run &next();
        bool has_next() const;
    };

    // Paragraph contains a paragraph
    // and stores runs
    class Paragraph {
    private:
        friend class Iterator<Paragraph>;
		// Store parent node (usually the body node)
        pugi::xml_node parent;
        // And store current node also
        pugi::xml_node current;
        // A paragraph consists of runs
        Run run;

    public:
        Paragraph();
        Paragraph(pugi::xml_node, pugi::xml_node);
        void set_parent(pugi::xml_node);
        void set_current(pugi::xml_node);

        Paragraph &next();
        bool has_next() const;

        Run &runs();
        Run &add_run(const std::string&);
        Run &add_run(const char*);
        Paragraph &insert_paragraph_after(const std::string&);
    };

	// TableCell contains one or more paragraphs
	class TableCell {
	private:
        friend class Iterator<TableCell>;
		pugi::xml_node parent;
		pugi::xml_node current;

		Paragraph paragraph;
	public:
		TableCell();
		TableCell(pugi::xml_node, pugi::xml_node);

		void set_parent(pugi::xml_node);
		void set_current(pugi::xml_node);

		Paragraph& paragraphs();

		TableCell& next();
		bool has_next() const;
	};

	// TableRow consists of one or more TableCells
	class TableRow {
        friend class Iterator<TableRow>;
		pugi::xml_node parent;
		pugi::xml_node current;

		TableCell cell;
	public:
		TableRow();
		TableRow(pugi::xml_node, pugi::xml_node);
		void set_parent(pugi::xml_node);
		void set_current(pugi::xml_node);

		TableCell& cells();

		bool has_next() const;
		TableRow& next();
	};

	// Table consists of one or more TableRow objects
	class Table {
	private:
        friend class Iterator<Table>;
		pugi::xml_node parent;
		pugi::xml_node current;

		TableRow row;
	public:
		Table();
		Table(pugi::xml_node, pugi::xml_node);
		void set_parent(pugi::xml_node);
		void set_current(pugi::xml_node);

		Table& next();
		bool has_next() const;

		TableRow& rows();
	};

    // Document contains whole the docx file
    // and stores paragraphs
    class Document {
    private:
        friend class Iterator<Document>;
        std::string directory;
        Paragraph paragraph;
		Table table;
        pugi::xml_document document;

    public:
        Document();
        Document(std::string);
        void file(std::string);
        void open();
        void save() const;

        Paragraph &paragraphs();
		Table& tables();
    };
}

#endif
